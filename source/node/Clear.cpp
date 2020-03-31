#include "rendergraph/node/Clear.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rendergraph
{
namespace node
{

void Clear::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (m_clear_type.empty()) {
        return;
    }

    auto rc = std::static_pointer_cast<RenderContext>(ctx);

    int flag = 0;
    for (auto& type : m_clear_type)
    {
        switch (type)
        {
        case Type::Color:
        {
            flag |= ur::MASKC;

            uint32_t argb = m_col.a << 24 | m_col.r << 16 | m_col.g << 8 | m_col.b;
            rc->rc.SetClearColor(argb);
        }
            break;
        case Type::Depth:
            flag |= ur::MASKD;
            break;
        case Type::Stencil:
            flag |= ur::MASKS;
            break;
        }
    }

    rc->rc.SetClearFlag(flag);
    rc->rc.Clear();
}

}
}