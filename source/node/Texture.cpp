#include "rendergraph/node/Texture.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/RenderSystem.h"
#include "rendergraph/ValueImpl.h"
#include "rendergraph/Evaluator.h"

#include <unirender/TextureTarget.h>
#include <unirender/TextureDescription.h>
#include <unirender/TextureMinificationFilter.h>
#include <unirender/TextureMagnificationFilter.h>
#include <unirender/TextureWrap.h>
#include <unirender/Texture.h>
#include <unirender/Device.h>

namespace rendergraph
{
namespace node
{

void Texture::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (IsDirty()) {
        auto rc = std::static_pointer_cast<RenderContext>(ctx);
        InitTexture(*rc);
    }

    if (!m_imports[I_SIZE].conns.empty())
    {
        auto rc = std::static_pointer_cast<RenderContext>(ctx);

        ShaderVariant var;
        uint32_t flags = 0;
        auto v_sz = Evaluator::Calc(*rc, m_imports[I_SIZE], var, flags);
        if (v_sz.type != VariableType::Any) {
            assert(v_sz.type == VariableType::Vector2);
            m_width  = static_cast<int>(v_sz.vec2.x);
            m_height = static_cast<int>(v_sz.vec2.y);
        }
    }
}

void Texture::Eval(const RenderContext& rc, size_t port_idx,
                   ShaderVariant& var, uint32_t& flags) const
{
    if (!m_imports[I_SIZE].conns.empty())
    {
        ShaderVariant var;
        uint32_t flags = 0;
        auto v_sz = Evaluator::Calc(rc, m_imports[I_SIZE], var, flags);
        if (v_sz.type != VariableType::Any) {
            assert(v_sz.type == VariableType::Vector2);
            const_cast<Texture*>(this)->m_width  = static_cast<int>(v_sz.vec2.x);
            const_cast<Texture*>(this)->m_height = static_cast<int>(v_sz.vec2.y);
        }
    }

    if (port_idx == O_OUT)
    {
        var.type = VariableType::Texture;

		auto tex = new TextureVal;
		tex->texture = m_tex;
		tex->sampler = m_sampler;
        var.p = tex;
    }
    else if (port_idx == O_SIZE)
    {
        var.type = VariableType::Vector2;
        if (m_tex) {
            var.vec2.Set(
                static_cast<float>(m_tex->GetWidth()), 
                static_cast<float>(m_tex->GetHeight())
            );
        } else {
            var.vec2.Set(0, 0);
        }
    }
}

void Texture::InitTexture(const RenderContext& rc) const
{
    ur::TextureTarget target;
    switch (m_type)
    {
    case Type::Tex2D:
        target = ur::TextureTarget::Texture2D;
        break;
    case Type::TexCube:
        target = ur::TextureTarget::TextureCubeMap;
        break;
    default:
        assert(0);
    }

    ur::TextureDescription desc;

    desc.width  = m_width;
    desc.height = m_height;

    switch (m_format)
    {
    case Format::RGBA16:
        desc.format = ur::TextureFormat::RGBA16F;
        break;
    case Format::RGBA8:
        desc.format = ur::TextureFormat::RGBA8;
        break;
    case Format::RGBA4:
        desc.format = ur::TextureFormat::RGBA4;
        break;
    case Format::RGB:
        desc.format = ur::TextureFormat::RGB;
        break;
    case Format::RGB565:
        desc.format = ur::TextureFormat::RGB565;
        break;
    case Format::RGBA32F:
        desc.format = ur::TextureFormat::RGBA32F;
        break;
    case Format::RGBA16F:
        desc.format = ur::TextureFormat::RGBA16F;
        break;
	case Format::RGB16F:
		desc.format = ur::TextureFormat::RGB16F;
		break;
	case Format::RG16F:
		desc.format = ur::TextureFormat::RG16F;
		break;
    case Format::A8:
        desc.format = ur::TextureFormat::A8;
        break;
    case Format::RED:
        desc.format = ur::TextureFormat::RED;
        break;
    case Format::Depth:
        desc.format = ur::TextureFormat::DEPTH;
        break;
    default:
        assert(0);
    }

    ur::TextureMinificationFilter min_filter;  // todo: mipmap
    ur::TextureMagnificationFilter mag_filter;
    switch (m_filter)
    {
    case Filtering::Nearest:
        min_filter = ur::TextureMinificationFilter::Nearest;
        mag_filter = ur::TextureMagnificationFilter::Nearest;
        break;
    case Filtering::Linear:
        min_filter = ur::TextureMinificationFilter::Linear;
        mag_filter = ur::TextureMagnificationFilter::Linear;
        break;
    default:
        assert(0);
    }

    ur::TextureWrap wrap_s;
    ur::TextureWrap wrap_t;
    switch (m_wrap)
    {
    case Wrapping::Repeat:
        wrap_s = ur::TextureWrap::Repeat;
        wrap_t = ur::TextureWrap::Repeat;
        break;
    case Wrapping::MirroredRepeat:
        wrap_s = ur::TextureWrap::MirroredRepeat;
        wrap_t = ur::TextureWrap::MirroredRepeat;
        break;
    case Wrapping::ClampToEdge:
        wrap_s = ur::TextureWrap::ClampToEdge;
        wrap_t = ur::TextureWrap::ClampToEdge;
        break;
    case Wrapping::ClampToBorder:
        wrap_s = ur::TextureWrap::ClampToBorder;
        wrap_t = ur::TextureWrap::ClampToBorder;
        break;
    default:
        assert(0);
    }

    if (!m_filepath.empty())
    {
        ;
    }
    else
    {
        switch (m_type)
        {
        case Type::Tex2D:
            desc.target = ur::TextureTarget::Texture2D;
            break;
        case Type::TexCube:
            desc.target = ur::TextureTarget::TextureCubeMap;
            break;
        }
    }

    m_tex = rc.ur_dev->CreateTexture(desc);
    m_sampler = rc.ur_dev->CreateTextureSampler(min_filter, mag_filter, wrap_s, wrap_t);
}

void Texture::Draw(const RenderContext& rc) const
{
    //if (shader) {
    //    //shader->Bind(rc);
    //    //rc.rc.RenderCube(ur::RenderContext::VertLayout::VL_POS_TEX);
    //} else {
        RenderSystem::Instance()->DrawTextureToScreen(rc, *m_tex);
    //}
}

}
}