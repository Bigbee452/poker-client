cmake_minimum_required(VERSION 3.15)

project(poker-client)

set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_SAMPLES OFF)

add_subdirectory(external/assimp)

find_package(glfw3 REQUIRED)

add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_subdirectory(external/glm)
add_subdirectory(src)

target_link_libraries(poker-client PRIVATE glad glfw assimp)