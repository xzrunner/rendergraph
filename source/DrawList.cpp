#include "rendergraph/DrawList.h"
#include "rendergraph/Node.h"
#include "rendergraph/node/ShaderGraph.h"
#include "rendergraph/node/Input.h"

#include <dag/Graph.h>

#include <assert.h>

#include <stack>
#include <queue>

namespace rendergraph
{

DrawList::DrawList(const std::vector<NodePtr>& all_nodes)
    : m_nodes(all_nodes)
{
	Sort();
}

void DrawList::GetAntecedentNodes(const NodePtr& src, std::vector<NodePtr>& nodes, bool only_ports)
{
    std::queue<NodePtr> buf;
    buf.push(src);

    while (!buf.empty())
    {
        auto n = buf.front(); buf.pop();
        nodes.push_back(n);
        for (auto& port : n->GetImports()) 
        {
            if (!only_ports || port.var.type.type == VariableType::Port) {
                for (auto& conn : port.conns) {
                    buf.push(std::static_pointer_cast<Node>(conn.node.lock()));
                }
            }
        }
    }
}

void DrawList::GetSubsequentNodes(const Node::Port& src, std::vector<NodePtr>& nodes, bool only_ports)
{
    std::queue<NodePtr> buf;
    for (auto& conn : src.conns) {
        buf.push(std::static_pointer_cast<Node>(conn.node.lock()));
    }

    while (!buf.empty())
    {
        auto n = buf.front(); buf.pop();
        nodes.push_back(n);
        for (auto& port : n->GetExports()) 
        {
            if (!only_ports || port.var.type.type == VariableType::Port) {
                for (auto& conn : port.conns) {
                    buf.push(std::static_pointer_cast<Node>(conn.node.lock()));
                }
            }
        }
    }
}

bool DrawList::Draw(const std::shared_ptr<dag::Context>& ctx, const Node* end) const
{
    bool finished = false;
    for (auto& n : m_nodes)
    {
        //if (n->IsEnable()) {
            n->Execute(ctx);
        //}
        if (n.get() == end) {
            finished = true;
            break;
        }
    }
    return finished;
}

void DrawList::SetInputVar(const std::string& name, const ur::TexturePtr& tex)
{
    for (auto& n : m_nodes)
    {        
        if (n->get_type() != rttr::type::get<node::Input>()) {
            continue;
        }

        auto input = std::static_pointer_cast<node::Input>(n);
        auto& conns = input->GetExports()[0].conns;
        if (conns.empty()) {
            continue;
        }

        for (auto& c : conns)
        {
            auto n = c.node.lock();
            if (!n) {
                continue;
            }

            if (n->get_type() == rttr::type::get<node::ShaderGraph>()) {
                std::static_pointer_cast<node::ShaderGraph>(n)->SetInputVar(name, tex);
            }
        }
    }
}

void DrawList::Sort()
{
    std::vector<std::shared_ptr<dag::Node<Variable>>> nodes(m_nodes.size());
    for (size_t i = 0, n = m_nodes.size(); i < n; ++i) {
        nodes[i] = m_nodes[i];
    }
    auto orders = dag::Graph<Variable>::TopologicalSorting(nodes);
	m_nodes.resize(orders.size());
    for (size_t i = 0, n = orders.size(); i < n; ++i) {
		m_nodes[i] = std::static_pointer_cast<Node>(nodes[orders[i]]);
    }
}

}