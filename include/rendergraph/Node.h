#pragma once

#include "rendergraph/Variable.h"

#include <rttr/registration>

#include <memory>
#include <vector>

namespace rg
{

class RenderContext;

class Node
{
public:
    Node();
    virtual ~Node() {}

    virtual void Execute(const RenderContext& rc) = 0;

    auto& GetImports() const { return m_imports; }
    auto& GetExports() const { return m_exports; }

    struct Port;
    void SetImports(const std::vector<Port>& imports) { m_imports = imports; }
    void SetExports(const std::vector<Port>& exports) { m_exports = exports; }

public:
	struct PortAddr
	{
        PortAddr() {}
		PortAddr(const std::shared_ptr<Node>& node, int idx)
			: node(node), idx(idx) {}
		PortAddr(const PortAddr& addr)
			: node(addr.node), idx(addr.idx) {}

		std::weak_ptr<Node> node;
		int idx = -1;
	};

	struct Port
	{
        Port() {}
        Port(const Variable& var)
            : var(var) {}

        Variable var;
		std::vector<PortAddr> conns;
	};

protected:
    std::vector<Port> m_imports, m_exports;

    RTTR_ENABLE()

}; // Node

void make_connecting(const Node::PortAddr& from, const Node::PortAddr& to);

}