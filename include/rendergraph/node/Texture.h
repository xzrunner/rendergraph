#pragma once

#include "rendergraph/Node.h"

#include <unirender/typedef.h>

namespace ur { class TextureSampler; }

namespace rendergraph
{
namespace node
{

class Texture : public Node
{
public:
    enum InputID
    {
        I_SIZE = 0,
    };

    enum OutputID
    {
        O_OUT = 0,
        O_SIZE,
    };

public:
    Texture()
    {
        m_imports = {
            {{ VariableType::Vector2, "size" }},
        };
        m_exports = {
            {{ VariableType::Texture, "out" }},
            {{ VariableType::Vector2, "size" }},
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override;

    void Init(const RenderContext& rc) const;

    void Draw(const RenderContext& rc) const;

    auto GetTexture() const { return m_tex; }
    void SetTexture(const ur::TexturePtr& tex) { m_tex = tex; }

    auto GetSampler() const { return m_sampler; }
    void SetSampler(const std::shared_ptr<ur::TextureSampler>& sampler) {
        m_sampler = sampler;
    }

    enum class Type
    {
        Tex2D,
        TexCube,
    };

    enum class Format
    {
        RGBA16,
        RGBA8,
        RGBA4,
        RGB,
        RGB565,
		RGB16F,
		RG16F,
        A8,
        RED,
        Depth,
    };

    enum class Wrapping
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
    };

    enum class Filtering
    {
        Nearest,
        Linear,
    };

private:
    mutable ur::TexturePtr m_tex = nullptr;
    mutable std::shared_ptr<ur::TextureSampler> m_sampler = nullptr;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Texture.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Texture

}
}