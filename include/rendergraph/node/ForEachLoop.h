#pragma once

#include "rendergraph/Node.h"

namespace rg
{

class DrawList;

namespace node
{

class ForEachLoop : public Node
{
public:
    enum InputID
    {
        I_PREV = 0,
        I_ARRAY,

        I_MAX_NUM
    };

    enum OutputID
    {
        O_NEXT = 0,
        O_ARRAY_ELEMENT,
        O_ARRAY_INDEX,
        O_LOOP_BODY,

        O_MAX_NUM
    };

public:
    ForEachLoop();
    virtual ~ForEachLoop();

    virtual void Execute(const RenderContext& rc) override;
    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override;

private:
    mutable int m_index_curr = 0;

    ShaderVariant m_array_var;

    std::unique_ptr<DrawList> m_body_dlist;

    RTTR_ENABLE(Node)

}; // ForEachLoop

}
}