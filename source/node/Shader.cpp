#include "rendergraph/node/Shader.h"

#include <unirender/Shader.h>
#include <unirender/VertexAttrib.h>

namespace rg
{
namespace node
{

void Shader::Execute(ur::RenderContext& rc)
{
    if (!m_shader && !m_vert.empty() && !m_frag.empty()) {
        std::vector<std::string> textures;
        CU_VEC<ur::VertexAttrib> va_list;
        m_shader = std::make_shared<ur::Shader>(
            &rc, m_vert.c_str(), m_frag.c_str(), textures, va_list, true
        );
    }
}

void Shader::SetCodes(const std::string& vert, const std::string& frag)
{
    if (m_vert == vert && m_frag == frag) {
        return;
    }

    m_vert = vert;
    m_frag = frag;

    m_shader.reset();
}

}
}