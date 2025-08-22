#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <glad/glad.h>

#include "indexBuffer.h"
#include "shader.h"
#include "vertexBuffer.h"
#include "../../include/stb_image.h"

using namespace std;

extern string execute_path;


class Texture {
public:
    unsigned int id;
    string type;
    string path;
    glm::vec2 size;
    Texture();
    Texture(const string& path, string type);
    Texture(unsigned char* source, glm::vec2 size);
private:
    void TextureFromFile(const string& path);
    void SetWhiteTexture();
};  

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    vector<Texture> textures;
}; 

class Mesh {
    public:
        // mesh data
        vector<Vertex>       vertices;
        vector<unsigned int> indices;
        vector<Texture>      textures;

        Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material& mat);
        void Draw(Shader &shader);
        void Draw(Shader &shader, bool enableTextures);
    private:
        //  render data
        vertex_buffer* vbo;
        index_buffer* ebo;

        Material material = {glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(0.5f), 32.0f};

        void setupMesh();
};  