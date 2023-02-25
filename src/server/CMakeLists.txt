project(${CMAKE_PROJECT_NAME}_lib)

file(GLOB_RECURSE SRC_FILES
    *.cpp
    EXCLUDE controllers/*.cpp
)

add_library(${PROJECT_NAME}
    ${SRC_FILES}
)

set(DATABASE_PATH database.db3)

target_compile_definitions(${PROJECT_NAME} PUBLIC PROJECT_NAME="${CMAKE_PROJECT_NAME}")
target_compile_definitions(${PROJECT_NAME} PRIVATE DATABASE_PATH="${DATABASE_PATH}")
target_compile_definitions(${PROJECT_NAME} PRIVATE DOCUMENT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/views/style")

if(UNIX)
    target_compile_options(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:-D_DEBUG -g -O0>"
        -Wall -Werror -Wno-unused-function -Wfatal-errors
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ABSOLUTE_EXTERNAL_PATH}/fmt/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    drogon
    fmt::fmt
    SQLiteCpp
)
