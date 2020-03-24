#include "rendergraph/node/Texture.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/RenderSystem.h"

#include <unirender/typedef.h>
#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>

namespace rendergraph
{
namespace node
{

Texture::~Texture()
{
    if (m_texid != 0) {
        ur::Blackboard::Instance()->GetRenderContext().ReleaseTexture(m_texid);
    }
}

void Texture::Eval(const RenderContext& rc, size_t port_idx,
                   ShaderVariant& var, uint32_t& flags) const
{
    if (port_idx == O_OUT)
    {
        // todo: other texture type
        var.type = VariableType::Sampler2D;
        var.res_id = m_texid;
    }
}

void Texture::Init(const RenderContext& rc)
{
    if (m_texid != 0) {
        return;
    }

    ur::TEXTURE_WRAP wrap;
    switch (m_wrap)
    {
    case Wrapping::Repeat:
        wrap = ur::TEXTURE_REPEAT;
        break;
    case Wrapping::MirroredRepeat:
        wrap = ur::TEXTURE_MIRRORED_REPEAT;
        break;
    case Wrapping::ClampToEdge:
        wrap = ur::TEXTURE_CLAMP_TO_EDGE;
        break;
    case Wrapping::ClampToBorder:
        wrap = ur::TEXTURE_CLAMP_TO_BORDER;
        break;
    }

    ur::TEXTURE_FILTER filter;
    switch (m_filter)
    {
    case Filtering::Nearest:
        filter = ur::TEXTURE_NEAREST;
        break;
    case Filtering::Linear:
        filter = ur::TEXTURE_LINEAR;
        break;
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
        {
            int format = 0;
            switch (m_format)
            {
            case Format::RGBA16:
                format = ur::TEXTURE_RGBA16F;
                break;
            case Format::RGBA8:
                format = ur::TEXTURE_RGBA8;
                break;
            case Format::RGBA4:
                format = ur::TEXTURE_RGBA4;
                break;
            case Format::RGB:
                format = ur::TEXTURE_RGB;
                break;
            case Format::RGB565:
                format = ur::TEXTURE_RGB565;
                break;
            case Format::A8:
                format = ur::TEXTURE_A8;
                break;
            case Format::RED:
                format = ur::TEXTURE_RED;
                break;
            case Format::Depth:
                format = ur::TEXTURE_DEPTH;
                break;
            }

            if (m_texid != 0) {
                rc.rc.ReleaseTexture(m_texid);
            }
            m_texid = rc.rc.CreateTexture(nullptr, m_width, m_height, format, 0, wrap, filter);
        }
            break;
        case Type::TexCube:
            if (m_texid != 0) {
                rc.rc.ReleaseTexture(m_texid);
            }
            m_texid = rc.rc.CreateTextureCube(m_width, m_height);
            break;
        }
    }
}

void Texture::Bind(const RenderContext& rc, int channel)
{
    rc.rc.BindTexture(m_texid, channel);
}

void Texture::Draw(const RenderContext& rc, std::shared_ptr<Shader>& shader) const
{
    if (shader) {
        shader->Bind(rc);
        rc.rc.RenderCube(ur::RenderContext::VertLayout::VL_POS_TEX);
    } else {
        RenderSystem::Instance()->DrawTextureToScreen(m_texid);
    }
}

}
}