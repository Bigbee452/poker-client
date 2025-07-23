#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <iostream>
#include "main.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "../include/stb_image.h"
#include "vertexBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::mat4 projection    = glm::mat4(1.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); 

    Shader defaultShader(DEFAULT_VERTEX_SHADER_PATH, DEFAULT_FRAGMENT_SHADER_PATH);
    Shader lightShader(DEFAULT_VERTEX_SHADER_PATH, DEFAULT_LIGHT_FRAGMENT_SHADER_PATH);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //top
        -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, //left
        0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, //bottom
        0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f //right
    };

    std::vector<Vertex> verices_vector = {};
    for(int i = 0; i < sizeof(vertices)/(6*sizeof(float)); i++){
        Vertex vertex;
        vertex.Position = glm::vec3(vertices[i], vertices[i+1], vertices[i+2]);
        vertex.Normal = glm::vec3(vertices[i+3], vertices[i+4], vertices[i+5]);
        verices_vector.push_back(vertex);
    }


    /*
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
        0, 2, 3, 
    }; 
    */

    unsigned int indices[] = {
        4, 2, 0,
        2, 7, 3,
        6, 5, 7,
        1, 7, 5,
        0, 3, 1,
        4, 1, 5,
        4, 6, 2,
        2, 6, 7,
        6, 4, 5,
        1, 3, 7,
        0, 2, 3,
        4, 0, 1
    };

    std::vector<unsigned int> indices_vertices;
    for(int i = 0; i < sizeof(indices)/sizeof(unsigned int); i++){
        indices_vertices.push_back(indices[i]);
    }

    Model myModel(DEFAULT_MODEL_PATH);

    glm::vec3 cameraPos = glm::vec3(20.0f, 0.0f, 0.0f);  

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

    Camera cam(cameraPos, cameraDirection);

    glm::vec3 lightPos(3.2f, 1.0f, 2.0f);

    stbi_set_flip_vertically_on_load(true);

    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        float camY = cos(2*glfwGetTime()) * radius;

        glm::vec3 cameraPos = glm::vec3(camX, camY, camZ);  

        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

        cam.set_pos(cameraPos);
        cam.set_direction(cameraDirection);

        glm::mat4 view = cam.get_view();

        // draw our first triangle
        defaultShader.bind();
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // retrieve the matrix uniform locations
        defaultShader.set_mat4("model", model);
        defaultShader.set_mat4("view", view);
        defaultShader.set_mat4("projection", projection);
        defaultShader.set_vec3("objectColor", 1.0f, 0.5f, 0.31f);
        defaultShader.set_vec3("lightColor",  1.0f, 1.0f, 1.0f);


        myModel.Draw(defaultShader);

        lightShader.bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); 
        lightShader.set_mat4("model", model);
        lightShader.set_mat4("view", view);
        lightShader.set_mat4("projection", projection);
        lightShader.set_vec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightShader.set_vec3("lightColor",  1.0f, 1.0f, 1.0f);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    glViewport(0, 0, width, height);
}

