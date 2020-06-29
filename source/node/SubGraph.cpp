#include "rendergraph/node/SubGraph.h"
#include "rendergraph/DrawList.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/node/Output.h"

namespace rendergraph
{
namespace node
{

SubGraph::SubGraph()
{
}

SubGraph::~SubGraph()
{
}

void SubGraph::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (m_dlist)
	{
		auto rc = std::static_pointer_cast<RenderContext>(ctx);
		rc->sub_graph_stack.push_back(this);
        m_dlist->Draw(ctx);
		rc->sub_graph_stack.pop_back();
    }
}

void SubGraph::Eval(const RenderContext& rc, size_t port_idx,
                    ShaderVariant& var, uint32_t& flags) const
{
	rc.sub_graph_stack.push_back(this);

	assert(port_idx >= 0 && port_idx < m_outputs.size());
    m_outputs[port_idx]->Eval(rc, 0, var, flags);

	rc.sub_graph_stack.pop_back();
}

void SubGraph::Setup(const std::shared_ptr<dag::Graph<Variable>>& graph,
	                 const std::vector<dag::Node<Variable>::Port>& inputs,
	                 const std::vector<dag::Node<Variable>::Port>& outputs)
{
	m_graph = graph;

	m_imports = inputs;
	m_exports = outputs;

	if (graph)
	{
		auto& nodes_map = graph->GetAllNodes();
		std::vector<NodePtr> nodes;
		nodes.reserve(nodes_map.size());
		for (auto& itr : nodes_map) {
			nodes.push_back(std::static_pointer_cast<Node>(itr.second));
		}
		m_dlist = std::make_unique<DrawList>(nodes);

		m_outputs.resize(outputs.size());
		for (int i = 0, n = outputs.size(); i < n; ++i) {
			for (auto& node : nodes) {
				if (node->get_type() == rttr::type::get<node::Output>() &&
					std::static_pointer_cast<node::Output>(node)->GetVarName() == outputs[i].var.type.name) {
					m_outputs[i] = node;
					break;
				}
			}
		}
	}
}

}
}