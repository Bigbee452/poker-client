#pragma once
#include <RmlUi/Core.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

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
};

class Gui {
public:
    void init(int width, int height);

private:
    // Instantiate the interfaces to RmlUi.
	OpenGLRenderInterface render_interface;
	Rml::SystemInterface system_interface;

    Rml::Context* context;
};