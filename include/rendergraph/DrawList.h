#pragma once

#include "rendergraph/Node.h"
#include "rendergraph/typedef.h"

#include <unirender/typedef.h>

#include <vector>

namespace rendergraph
{

class RenderContext;

class DrawList
{
public:
    DrawList(const std::vector<NodePtr>& all_nodes);

    bool Draw(const std::shared_ptr<dag::Context>& ctx,
        const Node* end = nullptr) const;

    void SetInputVar(const std::string& name, const ur::TexturePtr& tex);

    static void GetAntecedentNodes(const NodePtr& src, 
        std::vector<NodePtr>& nodes, bool only_ports = true);
    static void GetSubsequentNodes(const Node::Port& src, 
        std::vector<NodePtr>& nodes, bool only_ports = true);

private:
	void Sort();

private:
    std::vector<NodePtr> m_nodes;

}; // DrawList

}