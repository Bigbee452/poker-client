cmake_minimum_required(VERSION 3.15)

project(poker-client)

add_subdirectory(external/BBengine)
add_subdirectory(src)

target_link_libraries(poker-client PRIVATE PokerEngine assimp glad glm)