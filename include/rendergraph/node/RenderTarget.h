#pragma once

#include "rendergraph/Node.h"
#include "rendergraph/typedef.h"

namespace ur { class Framebuffer; }

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
        O_RT = 0,

		O_COLOR_TEX0,

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
        m_exports[O_RT] = {{ VariableType::RenderTarget, "out" }};
		m_exports[O_COLOR_TEX0] = { { VariableType::Texture, "col0" } };
    }

	virtual void Eval(const RenderContext& rc, size_t port_idx,
		ShaderVariant& var, uint32_t& flags) const override;

    void Bind(const RenderContext& rc);
    void Unbind(const RenderContext& rc);

    void SetSize(uint32_t width, uint32_t height);

private:
    void Setup(const RenderContext& rc);

    void SetupTexture(int input_idx, const RenderContext& rc);

private:
    std::shared_ptr<ur::Framebuffer> m_frame_buffer = nullptr;

	std::array<NodePtr, I_DEPTH_TEX - I_COLOR_TEX0 + 1> m_binded_textures;

    int m_prev_vp_x = 0, m_prev_vp_y = 0, m_prev_vp_w = 0, m_prev_vp_h = 0;
    std::shared_ptr<ur::Framebuffer> m_prev_fbo = nullptr;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/RenderTarget.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // RenderTarget

}
}