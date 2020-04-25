#include "rendergraph/node/Texture.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/RenderSystem.h"

#include <unirender2/TextureTarget.h>
#include <unirender2/TextureDescription.h>
#include <unirender2/TextureMinificationFilter.h>
#include <unirender2/TextureMagnificationFilter.h>
#include <unirender2/TextureWrap.h>
#include <unirender2/Texture.h>
#include <unirender2/Device.h>

namespace rendergraph
{
namespace node
{

void Texture::Eval(const RenderContext& rc, size_t port_idx,
                   ShaderVariant& var, uint32_t& flags) const
{
    if (port_idx == O_OUT)
    {
        // todo: other texture type
        if (m_tex)
        {
            var.type = VariableType::Sampler2D;
            var.p    = &m_tex;
        }
    }
}

void Texture::Init(const RenderContext& rc) const
{
    ur2::TextureTarget target;
    switch (m_type)
    {
    case Type::Tex2D:
        target = ur2::TextureTarget::Texture2D;
        break;
    case Type::TexCube:
        target = ur2::TextureTarget::TextureCubeMap;
        break;
    default:
        assert(0);
    }

    ur2::TextureDescription desc;

    desc.width  = m_width;
    desc.height = m_height;

    switch (m_format)
    {
    case Format::RGBA16:
        desc.format = ur2::TextureFormat::RGBA16F;
        break;
    case Format::RGBA8:
        desc.format = ur2::TextureFormat::RGBA8;
        break;
    case Format::RGBA4:
        desc.format = ur2::TextureFormat::RGBA4;
        break;
    case Format::RGB:
        desc.format = ur2::TextureFormat::RGB;
        break;
    case Format::RGB565:
        desc.format = ur2::TextureFormat::RGB565;
        break;
    case Format::A8:
        desc.format = ur2::TextureFormat::A8;
        break;
    case Format::RED:
        desc.format = ur2::TextureFormat::RED;
        break;
    case Format::Depth:
        desc.format = ur2::TextureFormat::DEPTH;
        break;
    default:
        assert(0);
    }

    ur2::TextureMinificationFilter min_filter;  // todo: mipmap
    ur2::TextureMagnificationFilter mag_filter;
    switch (m_filter)
    {
    case Filtering::Nearest:
        min_filter = ur2::TextureMinificationFilter::Nearest;
        mag_filter = ur2::TextureMagnificationFilter::Nearest;
        break;
    case Filtering::Linear:
        min_filter = ur2::TextureMinificationFilter::Linear;
        mag_filter = ur2::TextureMagnificationFilter::Linear;
        break;
    default:
        assert(0);
    }

    ur2::TextureWrap wrap_s;
    ur2::TextureWrap wrap_t;
    switch (m_wrap)
    {
    case Wrapping::Repeat:
        wrap_s = ur2::TextureWrap::Repeat;
        wrap_t = ur2::TextureWrap::Repeat;
        break;
    case Wrapping::MirroredRepeat:
        wrap_s = ur2::TextureWrap::MirroredRepeat;
        wrap_t = ur2::TextureWrap::MirroredRepeat;
        break;
    case Wrapping::ClampToEdge:
        wrap_s = ur2::TextureWrap::ClampToEdge;
        wrap_t = ur2::TextureWrap::ClampToEdge;
        break;
    case Wrapping::ClampToBorder:
        wrap_s = ur2::TextureWrap::ClampToBorder;
        wrap_t = ur2::TextureWrap::ClampToBorder;
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
            desc.target = ur2::TextureTarget::Texture2D;
            break;
        case Type::TexCube:
            desc.target = ur2::TextureTarget::TextureCubeMap;
            break;
        }
    }

    m_tex = rc.ur_dev->CreateTexture(desc);
}

void Texture::Bind(const RenderContext& rc, int channel)
{
    if (m_tex) {
        m_tex->Bind();
    }
}

void Texture::Draw(const RenderContext& rc, std::shared_ptr<Shader>& shader) const
{
    if (shader) {
        //shader->Bind(rc);
        //rc.rc.RenderCube(ur::RenderContext::VertLayout::VL_POS_TEX);
    } else {
        RenderSystem::Instance()->DrawTextureToScreen(rc, *m_tex);
    }
}

}
}