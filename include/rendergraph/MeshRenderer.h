#pragma once

#include "rendergraph/IRenderer.h"
#include "rendergraph/RendererImpl.h"

#include <SM_Vector.h>

#include <boost/noncopyable.hpp>

namespace pt0 { class Shader; }
namespace pt3 { class WindowContext; }

namespace rg
{

struct MeshVertex
{
    sm::vec3 pos;
    uint32_t col = 0;
};

class MeshRenderer : public IRenderer, private RendererImpl<MeshVertex>, private boost::noncopyable
{
public:
    MeshRenderer();

    virtual void Flush() override;

    void DrawLines(size_t num, const float* positions, uint32_t color);

    auto GetShader() { return m_shader; }

    void BindWindowContext(pt3::WindowContext& wc);

private:
    void InitShader();

private:
    ur::DRAW_MODE m_draw_mode = ur::DRAW_LINES;

}; // MeshRenderer

}