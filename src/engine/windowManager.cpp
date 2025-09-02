#include <iostream>
#include "windowManager.h"
#include "mesh.h"
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
    glfwSetKeyCallback(window, this->key_callback);
    glfwSetCharCallback(window, this->character_callback);
    glfwSetCursorPosCallback(window, this->cursor_position_callback);
    glfwSetMouseButtonCallback(window, this->mouse_button_callback);

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
    glFrontFace(GL_CW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    stbi_set_flip_vertically_on_load(true);

    gui = new Gui;
    gui->init(width, height);
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

    gui->render();

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
    gui->changeContextDimensions(width, height);
    glViewport(0, 0, width, height);
}

void Window::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool Window::getKey(int key){
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        gui->processKeyDown(key);
    }
}

void Window::character_callback(GLFWwindow* window, unsigned int codepoint){
    gui->processChar(codepoint);
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    gui->processMouseMove((int)xpos, (int)ypos);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(action == GLFW_PRESS){
        gui->processMouseButtonDown(button);
    } else if(action == GLFW_RELEASE){
        gui->processMouseButtonUp(button);
    }
}