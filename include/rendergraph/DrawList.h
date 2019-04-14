#pragma once

#include "rendergraph/typedef.h"

#include <vector>

namespace rg
{

class RenderContext;

class DrawList
{
public:
    DrawList(const NodePtr& node);
    DrawList(const std::vector<NodePtr>& all_nodes);

    bool Draw(const RenderContext& rc, NodePtr end = nullptr) const;

private:
    void PrepareNodes(const NodePtr& node);
    void TopologicalSorting();

private:
    std::vector<NodePtr> m_nodes;

}; // DrawList

}