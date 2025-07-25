add_executable(poker-client "main.cpp" "main.h" "vertexBuffer.cpp" "vertexBuffer.h" "indexBuffer.cpp" "indexBuffer.h" "utils.cpp" "utils.h" "shader.cpp" "shader.h" "camera.cpp" "camera.cpp" "mesh.cpp" "mesh.h" "model.cpp" "model.h" "../include/stb_image.cpp" "scene.cpp" "windowManager.cpp")

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