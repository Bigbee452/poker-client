#include "vertexBuffer.h"
#include <glad/glad.h>

vertex_buffer::vertex_buffer(){
    glGenVertexArrays(1, &vertex_array_id);
    glGenBuffers(1, &buffer_id);
}

vertex_buffer::~vertex_buffer(){
    glDeleteVertexArrays(1, &vertex_array_id);
    glDeleteBuffers(1, &buffer_id);
}

void vertex_buffer::set_data(std::vector<Vertex> vertices){
    glBindVertexArray(vertex_array_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    //texture coordinates
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    //color
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
}

void vertex_buffer::bind(){
    glBindVertexArray(vertex_array_id);
}

void vertex_buffer::unbind(){
    glBindVertexArray(0);    
}