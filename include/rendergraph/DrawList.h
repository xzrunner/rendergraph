#pragma once

#include "rendergraph/typedef.h"

#include <vector>

namespace ur { class RenderContext; }

namespace rg
{

class DrawList
{
public:
    DrawList(const NodePtr& node);

    void Draw(ur::RenderContext& rc) const;

private:
    void PrepareNodes(const NodePtr& node);
    void TopologicalSorting();

private:
    std::vector<NodePtr> m_nodes;

}; // DrawList

}