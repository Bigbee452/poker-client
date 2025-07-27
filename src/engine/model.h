#pragma once
#include <glm/fwd.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"

using namespace std;

class Model 
{
    public:
        Model(string path, string name);
        Model(string path, string name, Material* mat);
        void Draw(Shader &shader);	
        void setPosition(glm::vec3 position);
        void setPosition(float x, float y, float z);
        void setRotation(glm::vec3 rotation);
        void setRotation(float x, float y, float z);
        glm::vec3 getPosition();

        string name;
    private:
        // model data
        vector<Mesh> meshes;
        vector<Texture> textures_loaded; 
        string directory;

        glm::vec3 position;
        glm::vec3 rotation;

        void loadModel(string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             string typeName);

        Material* override_mat = nullptr;
};