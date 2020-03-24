#include "rendergraph/node/Clear.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rendergraph
{
namespace node
{

void Clear::Execute(const RenderContext& rc)
{
    if (m_clear_type == 0) {
        return;
    }

    int flag = 0;
    if (m_clear_type & CLEAR_COLOR)
    {
        flag |= ur::MASKC;
        uint32_t argb = m_col.a << 24 | m_col.r << 16 | m_col.g << 8 | m_col.b;
        rc.rc.SetClearColor(argb);
    }
    if (m_clear_type & CLEAR_DEPTH) {
        flag |= ur::MASKD;
    }
    if (m_clear_type & CLEAR_STENCIL) {
        flag |= ur::MASKS;
    }
    rc.rc.SetClearFlag(flag);
    rc.rc.Clear();
}

}
}