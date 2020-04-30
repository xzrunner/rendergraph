#include "rendergraph/node/Texture.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/RenderSystem.h"

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
}

void Texture::Bind(const RenderContext& rc, int channel)
{
    if (m_tex) {
        m_tex->Bind();
    }
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