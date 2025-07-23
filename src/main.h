#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define DEFAULT_VERTEX_SHADER_PATH "/home/sil/projects/Poker/poker-client/shaders/vert.glsl"
#define DEFAULT_FRAGMENT_SHADER_PATH "/home/sil/projects/Poker/poker-client/shaders/frag.glsl"
#define DEFAULT_LIGHT_FRAGMENT_SHADER_PATH "/home/sil/projects/Poker/poker-client/shaders/light_frag.glsl"
#define DEFAULT_MODEL_PATH "/home/sil/projects/Poker/poker-client/models/model.obj"
#define DEFAULT_LIGHT_MODEL "/home/sil/projects/Poker/poker-client/models/light.obj"

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 