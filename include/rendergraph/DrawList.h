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

    void Draw(const RenderContext& rc) const;

private:
    void PrepareNodes(const NodePtr& node);
    void TopologicalSorting();

private:
    std::vector<NodePtr> m_nodes;

}; // DrawList

}