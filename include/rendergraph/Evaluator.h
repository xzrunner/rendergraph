#pragma once

#include "rendergraph/ShaderVariant.h"
#include "rendergraph/Node.h"

namespace rg
{

class Evaluator
{
public:
    static ShaderVariant Calc(const RenderContext& rc,
        const Node::Port& in_port, VariableType type);
    static ShaderVariant Calc(const RenderContext& rc,
        const Node::Port& in_port, ShaderVariant expect);

private:
    static ShaderVariant DefaultValue(VariableType type);

}; // Evaluator

}