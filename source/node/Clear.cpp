#include "rendergraph/node/Clear.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void Clear::Execute(ur::RenderContext& rc)
{
    if (m_clear_type == 0) {
        return;
    }

    int flag = 0;
    if (m_clear_type & CLEAR_COLOR)
    {
        flag |= ur::MASKC;
        uint32_t argb = m_col.a << 24 | m_col.r << 16 | m_col.g << 8 | m_col.b;
        rc.SetClearColor(argb);
    }
    if (m_clear_type & CLEAR_DEPTH) {
        flag |= ur::MASKD;
    }
    if (m_clear_type & CLEAR_STENCIL) {
        flag |= ur::MASKS;
    }
    rc.SetClearFlag(flag);
    rc.Clear();
}

}
}