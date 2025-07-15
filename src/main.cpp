#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <iostream>
#include "main.h"
#include "camera.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "shader.h"
#include "camera.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //top
        -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, //left
        0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, //bottom
        0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f //right
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
        0, 2, 3, 
    }; 

    glm::vec3 cameraPos = glm::vec3(10.0f, 0.0f, 0.0f);  

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

    Camera cam(cameraPos, cameraDirection);

    vertex_buffer* buffer = new vertex_buffer();
    buffer->set_data(vertices, sizeof(vertices)/sizeof(float));

    index_buffer* indexBuffer = new index_buffer();
    indexBuffer->set_data(indices, sizeof(indices)/sizeof(unsigned int));

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
        buffer->bind(); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        indexBuffer->bind();
        glDrawElements(GL_TRIANGLES, indexBuffer->count, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    delete indexBuffer;
    delete buffer;

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

