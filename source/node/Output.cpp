#pragma once

#include "rendergraph/node/Output.h"

namespace rendergraph
{
namespace node
{

void Output::Eval(const RenderContext& rc, size_t port_idx, 
	              ShaderVariant& var, uint32_t& flags) const
{
	auto& conns = m_imports[0].conns;
	if (conns.empty()) {
		return;
	}

	assert(conns.size() == 1);

	auto node = conns[0].node.lock();
	assert(node->get_type().is_derived_from<Node>());
	std::static_pointer_cast<Node>(node)->Eval(rc, conns[0].idx, var, flags);
}

}
}