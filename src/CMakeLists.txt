project(${CMAKE_PROJECT_NAME})

set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/output/${CMAKE_BUILD_TYPE})
if(NOT EXISTS ${OUTPUT_DIR})
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
endif()

add_subdirectory(server)

# ISSUE: drogon's http controllers need to be compiled as the part of
# the executable instead of the library.
file(GLOB_RECURSE CONTROLLERS_FILES
    server/controllers/*.cpp
)

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    ${CONTROLLERS_FILES}
)

drogon_create_views(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/server/views
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    server
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_PROJECT_NAME}_lib
    drogon
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)