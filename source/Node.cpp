#include "rendergraph/Node.h"

#include <assert.h>

RTTR_REGISTRATION
{

rttr::registration::class_<rg::Node::Port>("rg::Node::Port")
	.property("var", &rg::Node::Port::var)
;

rttr::registration::class_<rg::Node>("rg::Node")
	.method("GetImports", &rg::Node::GetImports)
	.method("GetExports", &rg::Node::GetExports)
;

}

namespace rg
{

Node::Node()
{
}

void make_connecting(const Node::PortAddr& from, const Node::PortAddr& to)
{
	{
		auto node = from.node.lock();
		assert(node);
		auto& ports = node->GetExports();
		assert(from.idx >= 0 && from.idx < static_cast<int>(ports.size()));
		const_cast<Node::Port&>(ports[from.idx]).conns.push_back(to);
	}
	{
		auto node = to.node.lock();
		assert(node);
		auto& ports = node->GetImports();
		assert(to.idx >= 0 && to.idx < static_cast<int>(ports.size())
            && ports[to.idx].conns.empty());
		const_cast<Node::Port&>(ports[to.idx]).conns.push_back(from);
	}
}

}