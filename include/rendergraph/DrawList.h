#pragma once

#include "rendergraph/Node.h"
#include "rendergraph/typedef.h"

#include <vector>

namespace rg
{

class RenderContext;

class DrawList
{
public:
    DrawList(const std::vector<NodePtr>& all_nodes);

    bool Draw(const RenderContext& rc, NodePtr end = nullptr) const;

    static void GetAntecedentNodes(const NodePtr& src, std::vector<NodePtr>& nodes);
    static void GetSubsequentNodes(const Node::Port& src, std::vector<NodePtr>& nodes);

private:
    std::vector<NodePtr> CalcRealPath(const std::vector<NodePtr>& nodes);

    //void PrepareNodes(const NodePtr& end_node);
    void TopologicalSorting();

private:
    std::vector<NodePtr> m_nodes;

}; // DrawList

}