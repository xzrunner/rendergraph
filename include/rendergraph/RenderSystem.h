#pragma once

#include <cu/cu_macro.h>

#include <memory>

namespace ur { class Shader; }

namespace rendergraph
{

class RenderSystem
{
public:
    void DrawTextureToScreen(unsigned int tex_id) const;

private:
    void InitShader();

private:
    std::shared_ptr<ur::Shader> m_shader = nullptr;

    CU_SINGLETON_DECLARATION(RenderSystem)

}; // RenderSystem

}