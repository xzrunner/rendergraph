#include "rendergraph/node/Group.h"
#include "rendergraph/DrawList.h"

namespace rg
{
namespace node
{

Group::Group()
{
}

Group::~Group()
{
}

void Group::Execute(const RenderContext& rc)
{
    if (m_dlist) {
        m_dlist->Draw(rc);
    }
}

void Group::Eval(const RenderContext& rc, size_t port_idx,
                 ShaderVariant& var, uint32_t& flags) const
{
    m_outputs[port_idx].first->Eval(rc, m_outputs[port_idx].second, var, flags);
}

void Group::SetChildren(const std::vector<std::shared_ptr<Node>>& children)
{
    m_children = children;
    m_dlist = std::make_unique<DrawList>(children);
}

}
}