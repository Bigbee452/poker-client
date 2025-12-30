cmake_minimum_required(VERSION 3.15)

project(poker-client)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_subdirectory(external/BBEngine)

add_subdirectory(src)
target_include_directories(poker-client PRIVATE include)

target_link_libraries(poker-client PRIVATE glad glfw assimp SFML::Network RmlUi::Core RmlUi::Debugger glm::glm BBEngine)