#pragma once

#include "rendergraph/ShaderVariant.h"
#include "rendergraph/Node.h"

namespace rg
{

class Evaluator
{
public:
    static ShaderVariant Calc(const Node::Port& in_port, VariableType type);

private:
    static ShaderVariant DefaultValue(VariableType type);

}; // Evaluator

}