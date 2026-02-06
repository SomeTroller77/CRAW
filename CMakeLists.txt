cmake_minimum_required(VERSION 3.13.4)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
project(CRAW VERSION 0.1 DESCRIPTION "C Reddit API Wrapper" LANGUAGES C)
add_compile_options(-Wall -Wextra -Wpedantic -g)
file(GLOB SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

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

install(TARGETS CRAW
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include/CRAW
    FILES_MATCHING PATTERN "*.h"
)