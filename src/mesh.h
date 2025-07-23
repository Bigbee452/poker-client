#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <glad/glad.h>

#include "indexBuffer.h"
#include "shader.h"
#include "vertexBuffer.h"

using namespace std;

struct Texture {
    unsigned int id;
    string type;
    string path;
};  

class Mesh {
    public:
        // mesh data
        vector<Vertex>       vertices;
        vector<unsigned int> indices;
        vector<Texture>      textures;

        Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
        void Draw(Shader &shader);
    private:
        //  render data
        vertex_buffer* vbo;
        index_buffer* ebo;

        void setupMesh();
};  