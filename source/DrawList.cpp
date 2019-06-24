#include "rendergraph/DrawList.h"
#include "rendergraph/Node.h"

#include <assert.h>

#include <stack>
#include <queue>

namespace rg
{

DrawList::DrawList(const std::vector<NodePtr>& all_nodes)
    : m_nodes(all_nodes)
{
    TopologicalSorting();
}

void DrawList::GetAntecedentNodes(const NodePtr& src, std::vector<NodePtr>& nodes)
{
    std::queue<NodePtr> buf;
    buf.push(src);

    while (!buf.empty())
    {
        auto n = buf.front(); buf.pop();
        nodes.push_back(n);
        for (auto& port : n->GetImports()) {
            if (port.var.type == VariableType::Port) {
                for (auto& conn : port.conns) {
                    buf.push(conn.node.lock());
                }
            }
        }
    }
}

void DrawList::GetSubsequentNodes(const Node::Port& src, std::vector<NodePtr>& nodes)
{
    std::queue<NodePtr> buf;
    for (auto& conn : src.conns) {
        buf.push(conn.node.lock());
    }

    while (!buf.empty())
    {
        auto n = buf.front(); buf.pop();
        nodes.push_back(n);
        for (auto& port : n->GetExports()) {
            if (port.var.type == VariableType::Port) {
                for (auto& conn : port.conns) {
                    buf.push(conn.node.lock());
                }
            }
        }
    }
}

bool DrawList::Draw(const RenderContext& rc, NodePtr end) const
{
    bool finished = false;
    for (auto& n : m_nodes)
    {
        if (n->IsEnable()) {
            n->Execute(rc);
        }
        if (n == end) {
            finished = true;
            break;
        }
    }
    return finished;
}

//void DrawList::PrepareNodes(const NodePtr& end_node)
//{
//    // main path
//    std::vector<NodePtr> main_path;
//
//    std::queue<NodePtr> buf;
//    buf.push(end_node);
//    while (!buf.empty())
//    {
//        auto n = buf.front(); buf.pop();
//        main_path.push_back(n);
//        for (auto& port : n->GetImports()) {
//            if (port.var.type == VariableType::Port) {
//                for (auto& conn : port.conns) {
//                    buf.push(conn.node.lock());
//                }
//            }
//        }
//    }
//
//    // sub paths
//    std::set<NodePtr> nodes_set;
//    for (auto& n : main_path) {
//        nodes_set.insert(n);
//    }
//    for (auto& n : main_path) 
//    {
//        std::queue<NodePtr> buf;
//        buf.push(n);
//        while (!buf.empty())
//        {
//            auto n = buf.front(); buf.pop();
//            nodes_set.insert(n);
//            for (auto& port : n->GetExports()) {
//                if (port.var.type == VariableType::Port) {
//                    for (auto& conn : port.conns) {
//                        auto cnode = conn.node.lock();
//                        if (nodes_set.find(cnode) == nodes_set.end()) {
//                            buf.push(cnode);
//                        }
//                    }
//                }
//            }
//        }
//    }
//    
//    m_nodes.clear();
//    std::copy(nodes_set.begin(), nodes_set.end(), std::back_inserter(m_nodes));
//}

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