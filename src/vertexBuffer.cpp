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

void vertex_buffer::set_data(float* data, unsigned int count){
    glBindVertexArray(vertex_array_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

    glBufferData(GL_ARRAY_BUFFER, count*sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
}

void vertex_buffer::bind(){
    glBindVertexArray(vertex_array_id);
}

void vertex_buffer::unbind(){
    glBindVertexArray(0);    
}