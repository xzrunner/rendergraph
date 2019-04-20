#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class RenderTarget : public Node
{
public:
    RenderTarget()
    {
        m_imports = {
            {{ VariableType::Port,    "prev" }},
            {{ VariableType::Texture, "tex" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }
    virtual ~RenderTarget();

    virtual void Execute(const RenderContext& rc) override;

    void Bind(const RenderContext& rc);
    void Unbind(const RenderContext& rc);

    void SetSize(uint32_t width, uint32_t height) {
        m_width = width;
        m_height = height;
    }

    void EnableDepthRBO() { m_enable_rbo_depth = true; }
    void EnableColorRBO() { m_enable_rbo_color = true; }

private:
    uint32_t m_fbo = 0;

    bool m_binded = false;

    // last viewport
    int m_vp_x = 0, m_vp_y = 0, m_vp_w = 0, m_vp_h = 0;

    uint32_t m_width = 0;
    uint32_t m_height = 0;

    bool m_enable_rbo_depth = false;
    bool m_enable_rbo_color = false;

    uint32_t m_rbo_depth = 0;
    uint32_t m_rbo_color = 0;

    RTTR_ENABLE(Node)

}; // RenderTarget

}
}