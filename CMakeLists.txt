cmake_minimum_required(VERSION 3.13.4)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
project(CRAW VERSION 0.1 DESCRIPTION "C Reddit API Wrapper" LANGUAGES C)
option(BUILD_TESTING "Enable testing" OFF)
add_compile_options(-Wall -Wextra -Wpedantic -g)
file(GLOB SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)

find_package(CURL REQUIRED)
if(CURL_FOUND)
    message(STATUS "Found CURL version: ${CURL_VERSION_STRING}")
    message(STATUS "Using CURL include dir(s): ${CURL_INCLUDE_DIRS}")
    message(STATUS "Using CURL lib(s): ${CURL_LIBRARIES}")
    target_include_directories(${PROJECT_NAME} PRIVATE ${CURL_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
else()
    message(FATAL_ERROR "Could not find CURL")
endif()
find_package(PkgConfig REQUIRED)
pkg_check_modules(CJSON REQUIRED libcjson IMPORTED_TARGET)
if(CJSON_FOUND)
    message(STATUS "Found CJSON version: ${CJSON_VERSION_STRING}")
    message(STATUS "Using CJSON include dir(s): ${CJSON_INCLUDE_DIRS}")
    message(STATUS "Using CJSON lib(s): ${CJSON_LIBRARIES}")
    target_link_libraries(${PROJECT_NAME} PkgConfig::CJSON)
else()
    message(FATAL_ERROR "Could not find CURL")
endif()

install(TARGETS CRAW
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include/CRAW
    FILES_MATCHING PATTERN "*.h"
)

include(CTest)

if(BUILD_TESTING)
    enable_testing()
    add_executable(CRAW_Test_1 tests/CRAW_Test_1.c)
    target_link_libraries(CRAW_Test_1 CRAW curl)
    add_test(NAME CRAW_Test_1 COMMAND CRAW_Test_1)
endif()