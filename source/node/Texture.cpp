#include "rendergraph/node/Texture.h"

#include <unirender/typedef.h>
#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void Texture::Execute(ur::RenderContext& rc)
{
    if (!m_filepath.empty())
    {
        ;
    }
    else
    {
        switch (m_type)
        {
        case ur::TEXTURE_2D:
            rc.ReleaseTexture(m_texid);
            m_texid = rc.CreateTexture(nullptr, m_width, m_height, m_format);
            break;
        case ur::TEXTURE_CUBE:
            rc.ReleaseTexture(m_texid);
            m_texid = rc.CreateTextureCube(m_width, m_height);
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