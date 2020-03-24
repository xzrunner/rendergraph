#include "rendergraph/Node.h"

#include <assert.h>

RTTR_REGISTRATION
{

rttr::registration::class_<rendergraph::Node::Port>("rendergraph::Node::Port")
	.property("var", &rendergraph::Node::Port::var)
;

rttr::registration::class_<rendergraph::Node>("rendergraph::Node")
	.method("GetImports", &rendergraph::Node::GetImports)
	.method("GetExports", &rendergraph::Node::GetExports)
;

}

namespace rendergraph
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