#pragma once

#include "rendergraph/Node.h"

namespace rg
{

class DrawList;

namespace node
{

class Group : public Node
{
public:
    Group();
    virtual ~Group();

    virtual void Execute(const RenderContext& rc) override;
    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override;

    void SetChildren(const std::vector<std::shared_ptr<Node>>& children);

private:
    std::vector<std::shared_ptr<Node>> m_children;

    std::unique_ptr<DrawList> m_dlist;

    RTTR_ENABLE(Node)

}; // Group

}
}