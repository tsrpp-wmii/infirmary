cmake_minimum_required(VERSION 3.10)

project(infirmary VERSION 0.0.1 LANGUAGES CXX)

set(EXTERNAL "external")

add_subdirectory(${EXTERNAL}/drogon)

if(NOT DEFINED ${CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT UNIX AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
message(WARNING "Config with UNIX system and GNU compiler was only tested!")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(PROJECT_NAME="${CMAKE_PROJECT_NAME}")

if(UNIX)
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_compile_options(-D_DEBUG -g)
    endif()
    add_compile_options(-Wall -Werror -Wno-unused-function -Wfatal-errors)
endif()

add_subdirectory(src)

option(UNIT_TEST "Test the basic operation" ON)
if(UNIT_TEST)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    enable_testing()
    add_subdirectory(${EXTERNAL}/googletest)
    add_subdirectory(tests)
endif()