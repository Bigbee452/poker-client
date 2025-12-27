#pragma once
#include <RmlUi/Core.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <string>
#include <vector>

#include "RmlUi/Core/Context.h"
#include "mesh.h"
#include "shader.h"

#define GUI_VERTEX_SHADER_PATH "/shaders/gui_vert.glsl"
#define GUI_FRAGMENT_SHADER_PATH "/shaders/gui_frag.glsl"

static Shader* guiShader = nullptr;

class OpenGLRenderInterface : public Rml::RenderInterface
{
	Rml::CompiledGeometryHandle CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) override;

    void RenderGeometry(Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation, Rml::TextureHandle texture) override;

    void ReleaseGeometry(Rml::CompiledGeometryHandle geometry) override;

    Rml::TextureHandle LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source) override;

    Rml::TextureHandle GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i source_dimensions) override;

    void ReleaseTexture(Rml::TextureHandle texture) override;

    void EnableScissorRegion(bool enable) override;    

    virtual void SetScissorRegion(Rml::Rectanglei region) override;
 
    Texture* white_texture;

public:
    OpenGLRenderInterface();
};

class ButtonListener : public Rml::EventListener {
public:
    ButtonListener(bool* boolCallBack);
    void ProcessEvent(Rml::Event& event) override;
private:
    bool* Pressed;
};

class GuiObject {
public:
    GuiObject(Rml::Context* context, string path, string name);
    ~GuiObject();
    void makeDocument(Rml::Context* context);
    void showObject(bool show);
    void bindString(string name, string* bindString);
    void bindButton(bool* callback);

    string name = "None";
private:
    Rml::ElementDocument* document;
    Rml::Context* context;
    Rml::DataModelConstructor constructor;

    string path;
};

class Gui {
public:
    void init(int width, int height);
    void render();

    void addElement(string path, string name);
    void showElement(string name, bool show);
    void bindStringToElement(string name, string dataName, string* bindString);
    void initElement(string name);
    void deleteElement(string name);
    void bindButtonToElement(string name, bool* callback);

    void changeContextDimensions(int width, int height);
    void processKeyDown(int key);
    void processKeyUp(int key);
    void processChar(unsigned int codepoint);
    void processMouseMove(int x, int y);
    void processMouseButtonDown(int buttonIndex);
    void processMouseButtonUp(int buttonIndex);
private:
    // Instantiate the interfaces to RmlUi.
	OpenGLRenderInterface render_interface;
	Rml::SystemInterface system_interface;

    Rml::Context* context;

    int width;
    int height;

    vector<GuiObject*> objects;
};