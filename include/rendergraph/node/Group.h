#pragma once

#include "rendergraph/Node.h"
#include "rendergraph/typedef.h"

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
    void SetOutputs(const std::vector<std::pair<NodePtr, int>>& outputs) {
        m_outputs = outputs;
    }

private:
    std::vector<std::shared_ptr<Node>> m_children;
    std::vector<std::pair<NodePtr, int>> m_outputs;

    std::unique_ptr<DrawList> m_dlist;

    RTTR_ENABLE(Node)

}; // Group

}
}