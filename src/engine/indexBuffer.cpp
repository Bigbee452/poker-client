#include "indexBuffer.h"
#include <glad/glad.h>
#include <vector>

index_buffer::index_buffer(){
    glGenBuffers(1, &buffer_id);
}

index_buffer::~index_buffer(){
    glDeleteBuffers(1, &buffer_id);
}

void index_buffer::set_data(unsigned int* data, unsigned int in_count){
    count = in_count;
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void index_buffer::set_data(std::vector<unsigned int> indices){
    count = indices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void index_buffer::bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
}

void index_buffer::unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER ,buffer_id);    
}