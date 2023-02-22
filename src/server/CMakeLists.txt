project(${CMAKE_PROJECT_NAME}_lib)

file(GLOB_RECURSE SRC_FILES *.cpp)
file(GLOB_RECURSE H_FILES *.h)
file(GLOB_RECURSE HPP_FILES *.hpp)

add_library(${PROJECT_NAME}
    ${SRC_FILES}
    ${H_FILES}
    ${HPP_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${DROGON_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE drogon)