#pragma once

#include <cu/cu_macro.h>

#include <memory>

namespace ur { class ShaderProgram; class Texture; }

namespace rendergraph
{

class RenderContext;

class RenderSystem
{
public:
    void DrawTextureToScreen(const RenderContext& rc,
        const ur::Texture& tex) const;

private:
    mutable std::shared_ptr<ur::ShaderProgram> m_prog = nullptr;

    CU_SINGLETON_DECLARATION(RenderSystem)

}; // RenderSystem

}