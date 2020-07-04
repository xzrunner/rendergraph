#pragma once

#include "rendergraph/Node.h"

#include <unirender/typedef.h>

//namespace ur { class TextureSampler; }

namespace rendergraph
{
namespace node
{

class TextureCube : public Node
{
public:
    // loads a cubemap texture from 6 individual texture faces
    // order:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front)
    // -Z (back)
    enum InputID
    {
        I_RIGHT = 0,
        I_LEFT,
        I_TOP,
        I_BOTTOM,
        T_FRONT,
        I_BACK,

        I_MAX_NUM
    };

public:
    TextureCube()
    {
        m_imports = {
            {{ VariableType::Texture, "right" }},
            {{ VariableType::Texture, "left" }},
            {{ VariableType::Texture, "top" }},
            {{ VariableType::Texture, "bottom" }},
            {{ VariableType::Texture, "front" }},
            {{ VariableType::Texture, "back" }}
        };
        m_exports = {
            {{ VariableType::Texture, "out" }},
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override;

private:
	void BuildTexture(const RenderContext& rc) const;

private:
    mutable ur::TexturePtr m_tex = nullptr;

    RTTR_ENABLE(Node)

}; // TextureCube

}
}