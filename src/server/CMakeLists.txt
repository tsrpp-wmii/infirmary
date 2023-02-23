project(${CMAKE_PROJECT_NAME}_lib)

file(GLOB_RECURSE SRC_FILES *.cpp)

add_library(${PROJECT_NAME}
    ${SRC_FILES}
)

if(UNIX)
    target_compile_options(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:-D_DEBUG -g>"
        -Wall -Werror -Wno-unused-function -Wfatal-errors
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${EXTERNAL_ABSOLUTE_PATH}/fmt/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    drogon
    fmt::fmt
)