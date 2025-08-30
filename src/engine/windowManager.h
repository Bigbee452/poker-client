#pragma once
#include "scene.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gui.h"

class Window {
public:
    Window();
    ~Window();
    bool init(int width, int height);
    void setScene(Scene* in_scene);
    void render_frame();
    bool shouldStop();
    bool getKey(int key);

    static inline Gui* gui;
private:
    GLFWwindow* window;
    static inline Scene* scene;
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void processInput(GLFWwindow *window);
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void character_callback(GLFWwindow* window, unsigned int codepoint);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};