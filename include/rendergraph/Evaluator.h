#pragma once

#include "rendergraph/ShaderVariant.h"
#include "rendergraph/Node.h"

namespace rg
{

class Evaluator
{
public:
    static const uint32_t FLAG_MODEL_MAT = 0x0001;

    static ShaderVariant Calc(const RenderContext& rc,
        const Node::Port& in_port, VariableType type, uint32_t& flags);
    static ShaderVariant Calc(const RenderContext& rc,
        const Node::Port& in_port, ShaderVariant expect, uint32_t& flags);

private:
    static ShaderVariant DefaultValue(VariableType type);

}; // Evaluator

}