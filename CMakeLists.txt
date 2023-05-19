cmake_minimum_required(VERSION 3.0.0)
# Specify the project info.
project(CRAW VERSION 0.1 DESCRIPTION "C Reddit API Wrapper")
# Declare the library target.
add_library(${PROJECT_NAME} SHARED
    	src/CRAW_Main.c
    	src/CRAW.h
	src/CRAW_Account.c
	src/CRAW_Account.h
)
# Configure the directories to search for header files.
target_include_directories(${PROJECT_NAME} PRIVATE include)
# Set the version property.
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
# Set the shared object version property to the project's major version.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
# Set the public header property to the one with the actual API.
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER src/CRAW.h)
