#pragma once
#include "scene.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    ~Window();
    bool init(int width, int height);
    void setScene(Scene* in_scene);
    void render_frame();
    bool shouldStop();
private:
    GLFWwindow* window;
    static inline Scene* scene;
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void processInput(GLFWwindow *window);
};