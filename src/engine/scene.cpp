#include "scene.h"
#include "camera.h"
#include <glm/fwd.hpp>
#include <string>
#include "model.h"
#include "shader.h"

std::string execute_path;

Scene::Scene(std::string exePathString){
    exePath = exePathString;
    execute_path = exePathString;
    glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cam_dir = glm::vec3(1.0f, 0.0f, 0.0f);
    cam = new Camera(cam_pos, cam_dir);

    projection = glm::perspective(glm::radians(45.0f), (float)100 / (float)100, 0.1f, 100.0f);
    light = new Model(exePath+DEFAULT_LIGHT_MODEL, "light");

    defaultShader = new Shader(exePath+DEFAULT_VERTEX_SHADER_PATH, exePath+DEFAULT_FRAGMENT_SHADER_PATH);
    lightShader = new Shader(exePath+DEFAULT_VERTEX_SHADER_PATH, exePath+DEFAULT_LIGHT_FRAGMENT_SHADER_PATH);
}

Scene::~Scene(){
    delete cam;
    delete light;
    delete defaultShader;
    delete lightShader;

    for(Model* model : models){
        delete model;
    }
}

void Scene::add_model(std::string path, std::string name){
    Model* model = new Model(path, name);
    models.push_back(model);
}

void Scene::add_model(std::string path, std::string name, Material* mat){
    Model* model = new Model(path, name, mat);
    models.push_back(model);
}

Model* Scene::get_model(std::string name){
    for(Model* model : models){
        if(model->name == name){
            return model;
        }
    }
    return nullptr;
}

void Scene::draw(){
    light->setPosition(light_pos);

    glm::mat4 view = cam->get_view();

    lightShader->bind();
    lightShader->set_mat4("view", view);
    lightShader->set_mat4("projection", projection);

    light->Draw(*lightShader);
    defaultShader->bind();
    defaultShader->set_mat4("view", view);
    defaultShader->set_mat4("projection", projection);
    defaultShader->set_vec3("light_position",  light_pos);
    defaultShader->set_vec3("lightColor", light_color);
    glm::vec3 cam_pos = cam->get_pos();
    defaultShader->set_vec3("viewPos", cam_pos);

    for(Model* model : models){
        model->Draw(*defaultShader);
    }
}

void Scene::set_projection(float width, float height){
    projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);    
}