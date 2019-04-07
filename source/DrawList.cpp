#include "rendergraph/DrawList.h"
#include "rendergraph/Node.h"

#include <assert.h>

#include <stack>

namespace rg
{

DrawList::DrawList(const NodePtr& node)
{
    PrepareNodes(node);
    TopologicalSorting();
}

void DrawList::Draw(ur::RenderContext& rc) const
{
    for (auto& n : m_nodes) {
        n->Execute(rc);
    }
}

void DrawList::PrepareNodes(const NodePtr& node)
{
    m_nodes.push_back(node);

    auto& conns = node->GetImports()[0].conns;
    for (auto& c : conns) {
        auto prev = c.node.lock();
        if (prev) {
            PrepareNodes(prev);
        }
    }
}

void DrawList::TopologicalSorting()
{
    // prepare
    std::vector<int> in_deg(m_nodes.size(), 0);
    std::vector<std::vector<int>> out_nodes(m_nodes.size());
    for (int i = 0, n = m_nodes.size(); i < n; ++i)
    {
        auto& node = m_nodes[i];
        auto input_port = node->GetImports()[0];
        if (input_port.conns.empty()) {
            continue;
        }

        assert(input_port.conns.size() == 1);
        auto from = input_port.conns[0].node.lock();
        assert(from);
        for (int j = 0, m = m_nodes.size(); j < m; ++j) {
            if (from == m_nodes[j]) {
                in_deg[i]++;
                out_nodes[j].push_back(i);
                break;
            }
        }
    }

    // sort
    std::stack<int> st;
    std::vector<NodePtr> sorted;
    for (int i = 0, n = in_deg.size(); i < n; ++i) {
        if (in_deg[i] == 0) {
            st.push(i);
        }
    }
    while (!st.empty())
    {
        int v = st.top();
        st.pop();
        sorted.push_back(m_nodes[v]);
        for (auto& i : out_nodes[v]) {
            assert(in_deg[i] > 0);
            in_deg[i]--;
            if (in_deg[i] == 0) {
                st.push(i);
            }
        }
    }

    m_nodes = sorted;
}

}