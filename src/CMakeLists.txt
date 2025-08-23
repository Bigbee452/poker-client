set(ENGINE_SRC  "engine/camera.cpp"
                "engine/indexBuffer.cpp"
                "engine/mesh.cpp"
                "engine/model.cpp"
                "engine/scene.cpp"
                "engine/shader.cpp"
                "engine/utils.cpp"
                "engine/vertexBuffer.cpp"
                "engine/windowManager.cpp"
                "engine/windowManager.h"
                "engine/gui.cpp"
                "../include/stb_image.h"
                "../include/stb_image.cpp")

add_executable(poker-client "main.cpp" "main.h" "cards.cpp" "network.h" "network.cpp" ${ENGINE_SRC})

# Copy assets to the output directory
add_custom_command(TARGET poker-client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/models
        $<TARGET_FILE_DIR:poker-client>/models
)

add_custom_command(TARGET poker-client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:poker-client>/shaders
)

add_custom_command(TARGET poker-client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures
        $<TARGET_FILE_DIR:poker-client>/textures
)