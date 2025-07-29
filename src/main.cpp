#include "engine/windowManager.h"
#include "cards.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glm/fwd.hpp>
#include <iostream>


void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


void buildScene(Scene* scene){
    scene->add_model("models/table.fbx", "table");
    scene->add_model("models/deck.fbx", "deck");
    Model* table = scene->get_model("table");
    if(table){
        glm::vec3 position = table->getPosition();
        position.y -= 0.04f;
        table->setPosition(position);
        table->setRotation(0.0f, 0.0f, 0.0f);
    } else {
        std::cout << "didn't find object named table" << std::endl;
    }
}

int main(int argc, char* argv[])
{
    Scene* myScene;

    Window myWindow;
    myWindow.init(SCR_WIDTH, SCR_HEIGHT);

    std::filesystem::path exePath = std::filesystem::canonical(argv[0]).parent_path();

    glm::vec3 cameraPos = glm::vec3(3.0f, 1.0f, 0.0f);  
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

    myScene = new Scene(exePath);
    myWindow.setScene(myScene);
    myScene->set_projection(SCR_WIDTH, SCR_HEIGHT);

    myScene->cam->set_pos(cameraPos);
    myScene->cam->set_direction(cameraDirection);

    glm::vec3 lightPos(10.2f, 1.0f, 10.0f);
    myScene->light_pos = lightPos;

    buildScene(myScene);

    CardModel myModel(myScene);
    myModel.setPosition(0.0, 0.5, 0.0);

    // render loop
    // -----------
    while (!myWindow.shouldStop())
    {
        const float radius = 2.0f;
        float lightX = sin(glfwGetTime()) * radius;
        float lightZ = cos(glfwGetTime()) * radius;
        float lightY = 0.7f;

        lightPos = glm::vec3(lightX, lightY, lightZ);  

        myScene->light_pos = lightPos;


        myModel.setPosition(0.0f, 0.5+0.1*sin(2*glfwGetTime()), 0.0f);
        myModel.setRotation(glm::pi<float>()/2, 0, glfwGetTime());

        myWindow.render_frame();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    return 0;
}
