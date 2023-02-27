project(${CMAKE_PROJECT_NAME}_lib)

file(GLOB_RECURSE SRC_FILES
    *.cpp
    controllers/*.cpp
)

file(GLOB_RECURSE CONTROLLERS_FILES
    controllers/*.cpp
)

list(REMOVE_ITEM SRC_FILES ${CONTROLLERS_FILES})

add_library(${PROJECT_NAME}
    ${SRC_FILES}
)

set(DATABASE_PATH database.db3)
get_filename_component(ABSOLUTE_DATABASE_PATH ${DATABASE_PATH} ABSOLUTE)

target_compile_definitions(${PROJECT_NAME} PUBLIC PROJECT_NAME="${CMAKE_PROJECT_NAME}")
target_compile_definitions(${PROJECT_NAME} PUBLIC DOMAIN_NAME="$<IF:$<CONFIG:Debug>,localhost,infirmary.damian-tomczak.pl>")
target_compile_definitions(${PROJECT_NAME} PRIVATE DATABASE_PATH="${EXTERNAL_DATABASE_PATH}")
target_compile_definitions(${PROJECT_NAME} PRIVATE DOCUMENT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/views")
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<BOOL:${SQLITECPP_ENABLE_ASSERT_HANDLER}>:SQLITECPP_ENABLE_ASSERT_HANDLER>)

if(UNIX)
    target_compile_options(${PROJECT_NAME} PUBLIC
        $<$<NOT:$<CONFIG:Debug>>:-DNDEBUG>
        $<IF:$<CONFIG:Debug>, -g -Og, -O3>
        -Wall -Werror -Wno-unused-function -Wfatal-errors
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ABSOLUTE_EXTERNAL_PATH}/fmt/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    drogon
    fmt::fmt
    ssl
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    SQLiteCpp
)
