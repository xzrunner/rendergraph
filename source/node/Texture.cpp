#include "rendergraph/node/Texture.h"
#include "rendergraph/RenderContext.h"

#include <unirender/typedef.h>
#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void Texture::Execute(const RenderContext& rc)
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
        case ur::TEXTURE_2D:
            rc.rc.ReleaseTexture(m_texid);
            m_texid = rc.rc.CreateTexture(nullptr, m_width, m_height, m_format, 0, wrap, filter);
            break;
        case ur::TEXTURE_CUBE:
            rc.rc.ReleaseTexture(m_texid);
            m_texid = rc.rc.CreateTextureCube(m_width, m_height);
            break;
        }
    }
}

void Texture::SetParams(int type, int width, int height, int format)
{
    m_type   = type;
    m_width  = width;
    m_height = height;
    m_format = format;
}

}
}