#pragma once

#include "rendergraph/Variable.h"
#include "rendergraph/ShaderVariant.h"

#include <dag/Node.h>

#include <rttr/registration>

#include <memory>
#include <vector>

namespace rendergraph
{

class RenderContext;

class Node : public dag::Node<Variable>
{
public:
    Node() {}
    virtual ~Node() {}

    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const {}

//    bool IsEnable() const { return m_enable; }
//    void SetEnable(bool enable) { m_enable = enable; }
//
//private:
//    bool m_enable = false;
//
    RTTR_ENABLE(dag::Node<Variable>)

}; // Node

}