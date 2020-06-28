#pragma once

#include "rendergraph/node/Input.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/node/SubGraph.h"

namespace rendergraph
{
namespace node
{

void Input::Eval(const RenderContext& rc, size_t port_idx, 
	             ShaderVariant& var, uint32_t& flags) const
{
	if (rc.sub_graph_stack.empty()) {
		return;
	}

    auto sub_graph = rc.sub_graph_stack.back();
    for (auto& in : sub_graph->GetImports()) 
	{
		if (in.var.type.name != m_var_name) {
			continue;
		}
		if (!in.conns.empty()) 
		{
			assert(in.conns.size() == 1);
			auto& conn = in.conns[0];
			auto prev_node = conn.node.lock();
			assert(prev_node->get_type().is_derived_from<Node>());
			std::static_pointer_cast<Node>(prev_node)->Eval(rc, conn.idx, var, flags);
		}
		return;
    }
}

}
}