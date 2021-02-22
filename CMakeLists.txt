cmake_minimum_required(VERSION 3.0)

project(demo)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(src SRCS)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} ssl crypto )