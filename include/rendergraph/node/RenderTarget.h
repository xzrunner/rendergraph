#pragma once

#include "rendergraph/Node.h"

namespace ur { class RenderContext; }

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
            {{ VariableType::Texture, "color0 tex" }},
            {{ VariableType::Texture, "color1 tex" }},
            {{ VariableType::Texture, "color2 tex" }},
            {{ VariableType::Texture, "color3 tex" }},
            {{ VariableType::Texture, "depth tex" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }
    virtual ~RenderTarget();

    virtual void Execute(const RenderContext& rc) override;

    void Bind(const RenderContext& rc);
    void Unbind(const RenderContext& rc);

    void SetSize(uint32_t width, uint32_t height);

    void EnableDepthRBO();
    void EnableColorRBO();

    enum InputID
    {
        ID_COLOR0_TEX = 1,
        ID_COLOR1_TEX = 2,
        ID_COLOR2_TEX = 3,
        ID_COLOR3_TEX = 4,
        ID_DEPTH_TEX  = 5,
    };

private:
    void ExecuteTexture(int input_idx, ur::RenderContext& rc);
    bool BindTexture(int input_idx, ur::RenderContext& rc);

    void ReleaseRes();

private:
    uint32_t m_fbo = 0;

    bool m_binded = false;

    // last viewport
    int m_vp_x = 0, m_vp_y = 0, m_vp_w = 0, m_vp_h = 0;

    uint32_t m_width  = 0;
    uint32_t m_height = 0;

    bool m_enable_rbo_depth = false;
    bool m_enable_rbo_color = false;

    uint32_t m_rbo_depth = 0;
    uint32_t m_rbo_color = 0;

    RTTR_ENABLE(Node)

}; // RenderTarget

}
}