#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <iostream>
#include <filesystem>
#include "main.h"
#include "camera.h"
#include "camera.h"
#include "model.h"
#include "../include/stb_image.h"
#include "scene.h"
#include "windowManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Scene* myScene;

std::string execute_path;

int main(int argc, char* argv[])
{

    Window myWindow;
    myWindow.init(SCR_WIDTH, SCR_HEIGHT);

    std::filesystem::path exePath = std::filesystem::canonical(argv[0]).parent_path();
    execute_path = exePath;

    glm::vec3 cameraPos = glm::vec3(20.0f, 0.0f, 0.0f);  
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

    myScene = new Scene(exePath);
    myWindow.setScene(myScene);
    myScene->cam->set_pos(cameraPos);
    myScene->cam->set_direction(cameraDirection);

    glm::vec3 lightPos(10.2f, 1.0f, 2.0f);
    myScene->light_pos = lightPos;

    myScene->add_model(exePath/"models"/"deck.fbx");
    myScene->set_projection(SCR_WIDTH, SCR_HEIGHT);

    // render loop
    // -----------
    while (!myWindow.shouldStop())
    {
        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        float camY = 0.0f;

        glm::vec3 cameraPos = glm::vec3(camX, camY, camZ);  

        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

        myScene->cam->set_pos(cameraPos);
        myScene->cam->set_direction(cameraDirection);



        float objY = cos(2*glfwGetTime()) * 3.0f;
        myScene->models[0].setPosition(0.0f, objY, 1.0f);

        myWindow.render_frame();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
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
    myScene->set_projection(width, height);
    glViewport(0, 0, width, height);
}

