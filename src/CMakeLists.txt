project(${CMAKE_PROJECT_NAME}_lib)

file(GLOB_RECURSE SRC_FILES *.cpp)
file(GLOB_RECURSE H_FILES *.h)
file(GLOB_RECURSE HPP_FILES *.hpp)

get_filename_component(MAIN_PATH_CPP ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ABSOLUTE)
list(REMOVE_ITEM SRC_FILES "${MAIN_PATH_CPP}")

add_library(${PROJECT_NAME}
    ${SRC_FILES}
    ${H_FILES}
    ${HPP_FILES}
)

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_NAME})

set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/output/${CMAKE_BUILD_TYPE})
if(NOT EXISTS ${OUTPUT_DIR})
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
endif()

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)