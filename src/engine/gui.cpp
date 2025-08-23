#include "gui.h"


#include <iostream>
#include <vector>
#include "RmlUi/Core/Core.h"
#include "mesh.h"
#include "vertexBuffer.h"

Rml::CompiledGeometryHandle OpenGLRenderInterface::CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) {
    std::vector<Vertex> new_vertices;
    for(Rml::Vertex vertex : vertices){
        Vertex new_vertex;
        glm::vec3 position;
        position.x = vertex.position.x;
        position.y = vertex.position.y;
        position.z = 0;
        new_vertex.Position = position;
        glm::vec2 texcoords;
        texcoords.x = vertex.tex_coord.x;
        texcoords.y = vertex.tex_coord.y;
        new_vertex.TexCoords = texcoords;
        glm::vec4 color;
        color.r = vertex.colour.red;
        color.g = vertex.colour.green;
        color.b = vertex.colour.blue;
        color.a = vertex.colour.alpha;
        new_vertex.Color = color;
        new_vertices.push_back(new_vertex);
    }
    std::vector<unsigned int> new_indices;
    for(int index : indices){
        new_indices.push_back(index);
    }

    Mesh* handle = new Mesh(new_vertices, new_indices);
    Rml::CompiledGeometryHandle returnHandle = reinterpret_cast<Rml::CompiledGeometryHandle>(handle);
    return returnHandle;
}

void OpenGLRenderInterface::RenderGeometry(Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation, Rml::TextureHandle texture) {
    if(guiShader == nullptr){
        guiShader = new Shader(execute_path+GUI_VERTEX_SHADER_PATH, execute_path+GUI_FRAGMENT_SHADER_PATH);
        std::cout << "compiling gui shader" << std::endl;
    }
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(translation.x, translation.y, 0));
    guiShader->set_mat4("translate", transform);
    Mesh* mesh = reinterpret_cast<Mesh*>(geometry);
    glActiveTexture(GL_TEXTURE0);
    Texture* drawTexture = reinterpret_cast<Texture*>(texture);
    glBindTexture(GL_TEXTURE_2D, drawTexture->id);
    mesh->Draw(*guiShader, false);
}

void OpenGLRenderInterface::ReleaseGeometry(Rml::CompiledGeometryHandle geometry) {
    Mesh* mesh = reinterpret_cast<Mesh*>(geometry);
    delete mesh;     
} 

Rml::TextureHandle OpenGLRenderInterface::LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source) {
    Texture* texture = new Texture(source, "texture_gui");
    texture_dimensions.x = texture->size.x;
    texture_dimensions.y = texture->size.y;
    Rml::TextureHandle handle = reinterpret_cast<Rml::TextureHandle>(texture);
    return handle;
}

Rml::TextureHandle OpenGLRenderInterface::GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i source_dimensions) {
    unsigned char* data = new unsigned char[source.size()];
    for(unsigned long i = 0; i < source.size(); i++){
        data[i] = source[i];
    }
    Texture* texture = new Texture(data, glm::vec2(source_dimensions.x, source_dimensions.y));
    Rml::TextureHandle handle = reinterpret_cast<Rml::TextureHandle>(texture);
    return handle;
}

void OpenGLRenderInterface::ReleaseTexture(Rml::TextureHandle texture) {
    Texture* delete_texture = reinterpret_cast<Texture*>(texture);
    delete delete_texture;
}

void OpenGLRenderInterface::EnableScissorRegion(bool enable) {
    if (enable) {
        glEnable(GL_SCISSOR_TEST);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }        
}    

void OpenGLRenderInterface::SetScissorRegion(Rml::Rectanglei region) {
    glScissor(region.BottomLeft().x, region.BottomLeft().y, region.Width(), region.Height());
}

void Gui::init(int width, int height){
	// Begin by installing the custom interfaces.
	Rml::SetRenderInterface(&render_interface);
	Rml::SetSystemInterface(&system_interface);    

    Rml::Initialise();

    context = Rml::CreateContext("main", Rml::Vector2i(width, height));
	if (!context)
	{
		Rml::Shutdown();
        std::cerr << "failed to create Rml context" << std::endl;
	}

    Rml::LoadFontFace(execute_path + "fonts/old_font.ttf");    
}