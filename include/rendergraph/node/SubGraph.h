#pragma once

#include "rendergraph/Node.h"
#include "rendergraph/typedef.h"

#include <dag/Graph.h>

namespace rendergraph
{

class DrawList;

namespace node
{

class SubGraph : public Node
{
public:
    SubGraph();
    virtual ~SubGraph();

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;
    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override;

	void Setup(const std::shared_ptr<dag::Graph<Variable>>& graph,
		const std::vector<dag::Node<Variable>::Port>& inputs, 
		const std::vector<dag::Node<Variable>::Port>& outputs);

private:
	std::shared_ptr<dag::Graph<Variable>> m_graph = nullptr;

    std::vector<NodePtr> m_outputs;

    std::unique_ptr<DrawList> m_dlist;

    RTTR_ENABLE(Node)

}; // SubGraph

}
}