#include "rendergraph/node/Bind.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/node/Input.h"
#include "rendergraph/node/SubGraph.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void Bind::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (m_imports.empty() || m_imports[1].conns.empty()) {
        return;
    }

    auto node = m_imports[1].conns[0].node.lock();
	if (!node) {
		return;
	}

    node->Execute(ctx);

    auto rc = std::static_pointer_cast<RenderContext>(ctx);

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
						BindNode(in.conns[0].node.lock(), *rc);
						break;
					}
				}
			}
		}
	}
	else
	{
		BindNode(node, *rc);
	}
}

void Bind::BindNode(const std::shared_ptr<dag::Node<Variable>>& node, RenderContext& rc)
{
	auto type = node->get_type();
	if (type == rttr::type::get<node::RenderTarget>()) {
		std::static_pointer_cast<node::RenderTarget>(node)->Bind(rc);
	} else if (type == rttr::type::get<node::Shader>()) {
		std::static_pointer_cast<node::Shader>(node)->Bind(rc);
	}
}

}
}