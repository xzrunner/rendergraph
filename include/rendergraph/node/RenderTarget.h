#pragma once

#include "rendergraph/Node.h"

namespace ur { class RenderContext; }

namespace rendergraph
{
namespace node
{

class RenderTarget : public Node
{
public:
    enum InputID
    {
        I_COLOR_TEX0 = 0,
        I_COLOR_TEX1,
        I_COLOR_TEX2,
        I_COLOR_TEX3,
        I_DEPTH_TEX,

        I_MAX_NUM
    };

    enum OutputID
    {
        O_OUT = 0,

        O_MAX_NUM
    };

public:
    RenderTarget()
    {
        m_imports.resize(I_MAX_NUM);
        m_imports[I_COLOR_TEX0] = {{ VariableType::Texture, "col0" }};
        m_imports[I_COLOR_TEX1] = {{ VariableType::Texture, "col1" }};
        m_imports[I_COLOR_TEX2] = {{ VariableType::Texture, "col2" }};
        m_imports[I_COLOR_TEX3] = {{ VariableType::Texture, "col3" }};
        m_imports[I_DEPTH_TEX]  = {{ VariableType::Texture, "depth"  }};

        m_exports.resize(O_MAX_NUM);
        m_exports[O_OUT] = {{ VariableType::RenderTarget, "out" }};
    }
    virtual ~RenderTarget();

    void Bind(const RenderContext& rc);
    void Unbind(const RenderContext& rc);

    void SetSize(uint32_t width, uint32_t height);

    void EnableDepthRBO();
    void EnableColorRBO();

private:
    void Init(const RenderContext& rc);

    void InitTexture(int input_idx, ur::RenderContext& rc);
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