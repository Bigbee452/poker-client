cmake_minimum_required(VERSION 3.15)

project(poker-client)

find_package(glfw3 REQUIRED)

add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_subdirectory(src)

target_link_libraries(poker-client PRIVATE glad glfw)