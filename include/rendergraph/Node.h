#pragma once

#include "rendergraph/Variable.h"
#include "rendergraph/ShaderVariant.h"

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

    virtual void Execute(const RenderContext& rc) {}
    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const {}

    auto& GetImports() const { return m_imports; }
    auto& GetExports() const { return m_exports; }

    struct Port;
    void SetImports(const std::vector<Port>& imports) { m_imports = imports; }
    void SetExports(const std::vector<Port>& exports) { m_exports = exports; }

    bool IsEnable() const { return m_enable; }
    void SetEnable(bool enable) { m_enable = enable; }

public:
	struct PortAddr
	{
        PortAddr() {}
		PortAddr(const std::shared_ptr<Node>& node, int idx)
			: node(node), idx(idx) {}
		PortAddr(const PortAddr& addr)
			: node(addr.node), idx(addr.idx) {}

        bool operator == (const PortAddr& p) const
        {
            return node.lock() == p.node.lock()
                && idx == p.idx;
        }

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

private:
    bool m_enable = false;

    RTTR_ENABLE()

}; // Node

void make_connecting(const Node::PortAddr& from, const Node::PortAddr& to);

}