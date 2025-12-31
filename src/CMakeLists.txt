add_executable(poker-client "main.cpp" "main.h" "cards.cpp" "network.h" "network.cpp" "poker.cpp")

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

add_custom_command(TARGET poker-client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/fonts
        $<TARGET_FILE_DIR:poker-client>/fonts
)

add_custom_command(TARGET poker-client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/ui
        $<TARGET_FILE_DIR:poker-client>/ui
)