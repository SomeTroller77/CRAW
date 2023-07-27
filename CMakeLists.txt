cmake_minimum_required(VERSION 3.13.4)

project(CRAW VERSION 0.1 DESCRIPTION "C Reddit API Wrapper")
add_compile_options(-Wall -Wextra -Wpedantic -g)
add_library(${PROJECT_NAME} SHARED src/cJSON.c src/CRAW_Main.c src/CRAW_Account.c include/CRAW.h include/CRAW_Account.h include/cJSON.h include/CRAW_PRIVATE.h)

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

set_target_properties(CRAW PROPERTIES PUBLIC_HEADER "include/CRAW.h;include/CRAW_Account.h")

install(TARGETS CRAW LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/CRAW)


