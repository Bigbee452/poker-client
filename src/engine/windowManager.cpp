#include <iostream>
#include "windowManager.h"
#include "scene.h"

Window::Window(){
     
}

Window::~Window(){
    glfwTerminate();
}

bool Window::init(int width, int height){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation
    // --------------------
    window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, this->framebuffer_size_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);  
    glEnable(GL_CULL_FACE);  
    stbi_set_flip_vertically_on_load(true);
    return true;
}

void Window::setScene(Scene* in_scene){
    scene = in_scene;    
}

void Window::render_frame(){
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(scene != nullptr){
        scene->draw();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Window::shouldStop(){
    return glfwWindowShouldClose(window);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height){
    if(scene != nullptr){
        scene->set_projection(width, height);
    }
    glViewport(0, 0, width, height);
}

void Window::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}