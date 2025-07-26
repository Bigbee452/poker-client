#pragma once
#include "camera.h"
#include "model.h"
#include <glm/fwd.hpp>
#include <string>
#include <vector>

#define DEFAULT_VERTEX_SHADER_PATH "/shaders/vert.glsl"
#define DEFAULT_FRAGMENT_SHADER_PATH "/shaders/frag.glsl"
#define DEFAULT_LIGHT_FRAGMENT_SHADER_PATH "/shaders/light_frag.glsl"
#define DEFAULT_LIGHT_MODEL "/models/light.obj"

class Scene {
    public:
        Scene(std::string exePathString);
        ~Scene();
        glm::vec3 light_pos = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
        Camera* cam;
        std::vector<Model*> models = {};

        void add_model(std::string path, std::string name);
        void add_model(std::string path, std::string name, Material* mat);
        Model* get_model(std::string name);
        void draw();

        void set_projection(float width, float height);
        std::string exePath = "";

    private:
        Model* light;
        glm::mat4 projection;
        Shader* defaultShader;
        Shader* lightShader;
};