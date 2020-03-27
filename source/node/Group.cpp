#include "rendergraph/node/Group.h"
#include "rendergraph/DrawList.h"

namespace rendergraph
{
namespace node
{

Group::Group()
{
}

Group::~Group()
{
}

void Group::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (m_dlist) {
        m_dlist->Draw(ctx);
    }
}

void Group::Eval(const RenderContext& rc, size_t port_idx,
                 ShaderVariant& var, uint32_t& flags) const
{
    m_outputs[port_idx].first->Eval(rc, m_outputs[port_idx].second, var, flags);
}

void Group::SetChildren(const std::vector<NodePtr>& children,
                        const std::vector<std::pair<NodePtr, int>>& outputs)
{
    m_children = children;
    m_outputs  = outputs;

    //if (outputs.empty())
    //{
        m_dlist = std::make_unique<DrawList>(children);
    //}
    //else
    //{
    //    std::set<std::shared_ptr<Node>> path_nodes_set;

    //    for (auto& o : outputs)
    //    {
    //        std::vector<NodePtr> nodes;
    //        DrawList::GetAntecedentNodes(o.first, nodes);
    //        for (auto& n : nodes) {
    //            path_nodes_set.insert(n);
    //        }
    //    }

    //    std::vector<NodePtr> path_nodes;
    //    std::copy(path_nodes_set.begin(), path_nodes_set.end(), std::back_inserter(path_nodes));
    //    m_dlist = std::make_unique<DrawList>(path_nodes);
    //}
}

}
}