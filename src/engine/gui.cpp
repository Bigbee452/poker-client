#include "gui.h"


#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <vector>
#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/Core.h"
#include "RmlUi/Core/Input.h"
#include "RmlUi/Core/Math.h"
#include "RmlUi/Core/Types.h"
#include "mesh.h"
#include "vertexBuffer.h"

OpenGLRenderInterface::OpenGLRenderInterface(){
    white_texture = new Texture;
    guiShader = new Shader(execute_path+GUI_VERTEX_SHADER_PATH, execute_path+GUI_FRAGMENT_SHADER_PATH);
    std::cout << "compiled gui shader" << std::endl;
}

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
    //set transform of gui
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(translation.x, translation.y, 0));
    guiShader->bind();
    guiShader->set_mat4("translate", transform);

    //set the projection
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float vp_width = static_cast<float>(viewport[2]);
    float vp_height = static_cast<float>(viewport[3]);
    glm::mat4 projection = glm::ortho(0.0f, vp_width, 0.0f, vp_height);
    guiShader->set_mat4("projection", projection);

    //get mesh to render
    Mesh* mesh = reinterpret_cast<Mesh*>(geometry);
    glActiveTexture(GL_TEXTURE0);
    Texture* drawTexture; 
    if(texture != 0){   
        drawTexture = reinterpret_cast<Texture*>(texture);
    } else {
        drawTexture = white_texture;
    }
    glBindTexture(GL_TEXTURE_2D, drawTexture->id);
    guiShader->set_int("guiTexture", 0);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    mesh->Draw(*guiShader, false);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void OpenGLRenderInterface::ReleaseGeometry(Rml::CompiledGeometryHandle geometry) {
    Mesh* mesh = reinterpret_cast<Mesh*>(geometry);
    delete mesh;     
} 

Rml::TextureHandle OpenGLRenderInterface::LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source) {
    Texture* texture = new Texture(source, "texture_gui");
    texture_dimensions.x = texture->size.x;
    texture_dimensions.y = texture->size.y;
    std::cout << "load texture" << std::endl;
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
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float vp_height = static_cast<float>(viewport[3]);
    glScissor(region.BottomLeft().x, vp_height-region.BottomLeft().y, region.Width(), region.Height());
}

struct ApplicationData {
    bool show_text = true;
    Rml::String animal = "dog";
} my_data;

ButtonListener::ButtonListener(bool* boolCallBack){
    Pressed = boolCallBack;
}

void ButtonListener::ProcessEvent(Rml::Event& event) {
    if (event == "click") {
        *Pressed = true;
    }
}

GuiObject::GuiObject(Rml::Context* context, string path, string name){
    constructor = context->CreateDataModel(name);

    this->name = name;
    this->path = path;
    this->context = context;
}

GuiObject::~GuiObject(){
    context->RemoveDataModel(name);
    if(!document){
        return;
    }
    document->Close();
    
}

void GuiObject::makeDocument(Rml::Context* context){
    document = context->LoadDocument(path);    
    if(!document){
        cerr << "Failed to make GuiObject" << endl;
    }
}

void GuiObject::bindString(string name, string* bindString){
    constructor.Bind(name, bindString);
}

void GuiObject::showObject(bool show){
    if(!document){
        return;
    }
    if(show){
        document->Show();
        return;
    }
    document->Hide();
}

void GuiObject::bindButton(bool* callback){
    Rml::Element* button = document->GetElementById("manageClick");
    if(button){
        button->AddEventListener("click", new ButtonListener(callback));
    } else {
        std::cout << "No buttons found" << endl;
    }
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

    struct FontFace {
		const char* filename;
		bool fallback_face;
	};

	FontFace font_faces[] = {
		{"LatoLatin-Regular.ttf", false},
		{"LatoLatin-Italic.ttf", false},
		{"LatoLatin-Bold.ttf", false},
		{"LatoLatin-BoldItalic.ttf", false},
		{"NotoEmoji-Regular.ttf", true},
	};

    for (const FontFace& face : font_faces){
        Rml::LoadFontFace(execute_path + "/fonts/" + face.filename, face.fallback_face);
    }    
}

void Gui::render(){
    context->Update();
    context->Render();
}

void Gui::addElement(string path, string name){
    for(GuiObject* element : objects){
        if(element->name == name){
            std::cerr << "trying to add GUI element with the same name as another. name: " << name << endl;
            return;
        }
    }
    GuiObject* object = new GuiObject(context, path, name);
    objects.push_back(object);
}

void Gui::initElement(string name){
    for(GuiObject* element : objects){
        if(element->name == name){
            element->makeDocument(context);
            return;
        }
    }    
}

void Gui::deleteElement(string name){
    for(int i = 0; i < objects.size(); i++){
        if(objects[i]->name == name){
            delete objects[i];
            objects.erase(objects.begin()+i);
        }
    }  
}

void Gui::bindStringToElement(string name, string dataName, string* bindString){
    for(GuiObject* element : objects){
        if(element->name == name){
            element->bindString(dataName, bindString);
            return;
        }
    }       
}

void Gui::bindButtonToElement(string name, bool* callback){
    for(GuiObject* element : objects){
        if(element->name == name){
            element->bindButton(callback);
            return;
        }
    }       
}

void Gui::showElement(string name, bool show){
    for(GuiObject* element : objects){
        if(element->name == name){
            element->showObject(show);
            return;
        }
    }    
}

void Gui::changeContextDimensions(int width, int height){
    context->SetDimensions(Rml::Vector2i(width, height));
}

void Gui::processKeyDown(int key){
    Rml::Input::KeyIdentifier KeyIdentifier;
    if(key == 258){
        KeyIdentifier = Rml::Input::KI_TAB;
    } else if(key == 259){
        KeyIdentifier = Rml::Input::KI_BACK;
    }
    context->ProcessKeyDown(KeyIdentifier, 0);
}

void Gui::processKeyUp(int key){
    Rml::Input::KeyIdentifier KeyIdentifier;
    if(key == 258){
        KeyIdentifier = Rml::Input::KI_TAB;
    } else if(key == 259){
        KeyIdentifier = Rml::Input::KI_BACK;
    }

    context->ProcessKeyUp(KeyIdentifier, 0);
}

void Gui::processChar(unsigned int codepoint){
    context->ProcessTextInput((Rml::Character)codepoint);
}

void Gui::processMouseMove(int x, int y){
    context->ProcessMouseMove(x, y, 0);
}

void Gui::processMouseButtonUp(int buttonIndex){
    if(buttonIndex < 3){
        context->ProcessMouseButtonUp(buttonIndex, 0);
    }
}

void Gui::processMouseButtonDown(int buttonIndex){
    if(buttonIndex < 3){
        context->ProcessMouseButtonDown(buttonIndex, 0);
    }
}