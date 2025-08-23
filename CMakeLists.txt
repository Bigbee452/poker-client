cmake_minimum_required(VERSION 3.15)

project(poker-client)

set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_SAMPLES OFF)

add_subdirectory(external/assimp)

find_package(glfw3 REQUIRED)

add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

set(SFML_BUILD_AUDIO FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_SYSTEM TRUE CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK TRUE CACHE BOOL "" FORCE)
set(SFML_USE_SYSTEM_DEPS TRUE CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_subdirectory(external/glm)

set(RMLUI_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(RMLUI_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/RmlUi)

add_subdirectory(src)
target_include_directories(poker-client PRIVATE include)

target_link_libraries(poker-client PRIVATE glad glfw assimp SFML::Network RmlUi::Core RmlUi::Debugger)