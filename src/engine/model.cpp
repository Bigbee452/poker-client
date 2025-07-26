#include "model.h"
#include "assimp/material.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <iostream>

void Model::setPosition(glm::vec3 position){
    this->position = position;
}

void Model::setPosition(float x, float y, float z){
    setPosition(glm::vec3(x, y, z));
}

glm::vec3 Model::getPosition(){
    return position;
}

Model::Model(std::string path, std::string name)
{
    loadModel(path);
    this->name = name;
}
Model::Model(std::string path, std::string name, Material* mat)
{
    this->name = name;
    this->override_mat = mat;
    loadModel(path);
}

void Model::Draw(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        
    model = glm::translate(model, position);
    // retrieve the matrix uniform locations
    shader.set_mat4("model", model);
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
} 


void Model::loadModel(string path)
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);

    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	
	
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}  

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));			
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
} 

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].z;
        vector.z = mesh->mVertices[i].y;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f); 
        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);        
    }

    if(override_mat != nullptr){
        return Mesh(vertices, indices, *override_mat);
    }


    if(mesh->mMaterialIndex >= 0)
    {
        Material mat = {glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(0.5f), 32.0f};
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D color(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color); 
        mat.diffuse.r = color.r;
        mat.diffuse.g = color.g;
        mat.diffuse.b = color.b;

        material->Get(AI_MATKEY_COLOR_AMBIENT, color); 
        mat.ambient.r = color.r;
        mat.ambient.g = color.g;
        mat.ambient.b = color.b;

        material->Get(AI_MATKEY_COLOR_SPECULAR, color); 
        mat.specular.r = color.r;
        mat.specular.g = color.g;
        mat.specular.b = color.b;

        vector<Texture> diffuseMaps = loadMaterialTextures(material, 
                                        aiTextureType_DIFFUSE, "texture_diffuse");
        mat.textures = diffuseMaps;
        return Mesh(vertices, indices, mat);
    } 
    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices);
}


vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}