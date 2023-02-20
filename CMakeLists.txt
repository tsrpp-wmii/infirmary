cmake_minimum_required(VERSION 3.10)

project(infirmary VERSION 0.0.1 LANGUAGES CXX)

add_subdirectory(external/drogon EXCLUDE_FROM_ALL)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT UNIX AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
message(WARNING "Config with UNIX system and GNU compiler was only tested!")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(PROJECT_NAME="${CMAKE_PROJECT_NAME}")

option(UNIT_TEST "Test the basic operation" ON)

add_subdirectory(src)
if(UNIT_TEST)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    enable_testing()
    add_subdirectory(external/googletest EXCLUDE_FROM_ALL)
    add_subdirectory(tests)
endif()