#include "rendergraph/node/Unbind.h"
#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/node/Input.h"
#include "rendergraph/node/SubGraph.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Context.h>

namespace rendergraph
{
namespace node
{

void Unbind::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (m_imports.empty() || m_imports[0].conns.empty()) {
        return;
    }

    if (m_imports[1].conns.empty()) {
        return;
    }
    auto node = m_imports[1].conns[0].node.lock();
    if (!node) {
		return;
    }

	auto rc = std::static_pointer_cast<RenderContext>(ctx);

	auto type = node->get_type();
	if (node->get_type() == rttr::type::get<node::Input>())
	{
		auto rc = std::static_pointer_cast<RenderContext>(ctx);
		if (!rc->sub_graph_stack.empty()) 
		{
			auto input = std::static_pointer_cast<node::Input>(node);
			auto sub_graph = rc->sub_graph_stack.back();
			for (auto& in : sub_graph->GetImports()) {
				if (in.var.type.name == input->GetVarName()) {
					if (!in.conns.empty()) {
						rc->ur_ctx->SetFramebuffer(nullptr);
						break;
					}
				}
			}
		}
	}
	else
	{
		rc->ur_ctx->SetFramebuffer(nullptr);
	}
}

}
}