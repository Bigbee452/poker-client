#pragma once
#include <vector>
#include <glm/glm.hpp>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    //texture coordinate
    glm::vec2 TexCoords;
};

class vertex_buffer {
    public:
        vertex_buffer();
        ~vertex_buffer();
        void set_data(std::vector<Vertex> vertices);
        void bind();
        void unbind();
    private:
        unsigned int buffer_id;
        unsigned int vertex_array_id;
};