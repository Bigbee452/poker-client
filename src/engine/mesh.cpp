#include "mesh.h"
#include "indexBuffer.h"
#include "vertexBuffer.h"
#include <glm/fwd.hpp>
#include <iostream>
#include <string>
#include <vector>

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->indices = indices;

    Texture texture;
    texture.type = "texture_diffuse";
    texture.path = "default/white_texture";
    this->textures = {texture};

    setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material& mat) : material(mat) {
    this->vertices = vertices;
    this->indices = indices;
    if(!mat.textures.empty()){
        this->textures = mat.textures;
    } else {
        Texture texture;
        texture.type = "texture_diffuse";
        texture.path = "default/white_texture";
        this->textures = {texture};        
    }

    setupMesh();    
}

void Mesh::setupMesh()
{
    vbo = new vertex_buffer;
    vbo->set_data(vertices);
    ebo = new index_buffer;
    ebo->set_data(indices);

    vbo->bind();
    ebo->bind();
} 

void Mesh::Draw(Shader &shader){
    Draw(shader, true);
}

void Mesh::Draw(Shader &shader, bool enableTextures) 
{
    shader.set_vec3("material.ambient", material.ambient);
    shader.set_vec3("material.diffuse", material.diffuse);
    shader.set_vec3("material.specular", material.specular);
    shader.set_float("material.shininess", 32.0f);
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    if(enableTextures){
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);

            shader.set_int(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);
    }

    // draw mesh
    vbo->bind();
    ebo->bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
} 

void Texture::TextureFromFile(const string& path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    id = textureID;
    size = glm::vec2(width, height);
}

void Texture::SetWhiteTexture(){
    unsigned int textureID;
    glGenTextures(1, &textureID);

    unsigned char* data = new unsigned char[3];
    data[0] = 255;
    data[1] = 255;
    data[2] = 255;    

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] data;
    id = textureID;
    size = glm::vec2(1, 1);
}

Texture::Texture(){
    SetWhiteTexture();
}

Texture::Texture(const string& path, string type){
    TextureFromFile(path);
    this->type = type;
    this->path = path;
}

Texture::Texture(unsigned char* source, glm::vec2 size){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] source;
    id = textureID;
    this->size = size;
}