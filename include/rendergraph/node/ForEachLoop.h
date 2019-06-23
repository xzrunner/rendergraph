#pragma once

#include "rendergraph/Node.h"

namespace rg
{
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
    ForEachLoop()
    {
        m_imports.resize(I_MAX_NUM);
        m_imports[I_PREV]      = {{ VariableType::Port, "prev" }};
        m_imports[I_ARRAY]     = {{ VariableType::Any,  "array" }};

        m_exports.resize(O_MAX_NUM);
        m_exports[O_NEXT]          = {{ VariableType::Port, "next" }};
        m_exports[O_ARRAY_ELEMENT] = {{ VariableType::Any,  "array_element" }};
        m_exports[O_ARRAY_INDEX]   = {{ VariableType::Any,  "array_index" }};
        m_exports[O_LOOP_BODY]     = {{ VariableType::Port, "loop_body" }};
    }

    virtual void Execute(const RenderContext& rc) override;
    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override;

    //void SetProps(int i_begin, int i_end) {
    //    m_index_begin = i_begin;
    //    m_index_end   = i_end;
    //    m_index_curr  = 0;
    //}

private:
    //int m_index_begin = 0;
    //int m_index_end   = 0;

    //int m_index_max = 0;

    mutable int m_index_curr = 0;

    RTTR_ENABLE(Node)

}; // ForEachLoop

}
}