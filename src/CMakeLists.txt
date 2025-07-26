add_executable(poker-client "main.cpp")

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