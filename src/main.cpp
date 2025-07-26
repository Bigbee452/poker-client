#include "engine/mesh.h"
#include "engine/windowManager.h"
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
    Material* mat = new Material;
    mat->diffuse = glm::vec3(1.0f);
    mat->specular = glm::vec3(1.0f);
    mat->ambient = glm::vec3(1.0f);
    mat->shininess = 32.0f;
    Texture texture;
    texture.id = TextureFromFile("textures/cards/2_of_clubs.png", execute_path);
    texture.type = "texture_diffuse";
    texture.path = "textures/cards/2_of_clubs.png/donotuse";
    std::vector<Texture> textures = {texture};
    mat->textures = textures;
    scene->add_model("models/card.fbx", "card", mat);
    Model* table = scene->get_model("table");
    if(table != nullptr){
        glm::vec3 position = table->getPosition();
        position.y -= 0.04f;
        table->setPosition(position);
    } else {
        std::cout << "didn't find object named table" << std::endl;
    }

    Model* card = scene->get_model("card");
    if(card != nullptr){
        glm::vec3 position = card->getPosition();
        position.y += 0.3;
        card->setPosition(position);
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

        myWindow.render_frame();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    return 0;
}
