cmake_minimum_required(VERSION 3.10)

project(infirmary VERSION 0.0.1 LANGUAGES CXX)

if(NOT UNIX AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(WARNING "Config was only tested on UNIX system and GNU compiler!")
endif()

if(NOT DEFINED ${CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(EXTERNAL_PATH "external")
get_filename_component(ABSOLUTE_EXTERNAL_PATH ${EXTERNAL_PATH} ABSOLUTE)
set(DATA_PATH "data")
get_filename_component(ABSOLUTE_DATA_PATH ${DATA_PATH} ABSOLUTE)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${ABSOLUTE_EXTERNAL_PATH}/drogon)

# TODO: find irrevelant targets
add_subdirectory(${ABSOLUTE_EXTERNAL_PATH}/fmt)

set(SQLITECPP_ENABLE_ASSERT_HANDLER ON CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPCHECK OFF CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPLINT OFF CACHE BOOL "" FORCE)
add_subdirectory(${ABSOLUTE_EXTERNAL_PATH}/SQLiteCpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)

option(UNIT_TEST "Test the basic operation" ON)
if(UNIT_TEST)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    enable_testing()
    add_subdirectory(${ABSOLUTE_EXTERNAL_PATH}/googletest EXCLUDE_FROM_ALL)
    add_subdirectory(tests)
endif()