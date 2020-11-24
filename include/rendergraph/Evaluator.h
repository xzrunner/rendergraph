#pragma once

#include "rendergraph/ShaderVariant.h"
#include "rendergraph/Node.h"

namespace rendergraph
{

class Evaluator
{
public:
    static const uint32_t FLAG_MODEL_MAT  = 0x0001;
    static const uint32_t FLAG_VIEW_MAT   = 0x0002;
    static const uint32_t FLAG_PROJ_MAT   = 0x0004;
    static const uint32_t FLAG_CAMERA_POS = 0x0008;

    static ShaderVariant Calc(const RenderContext& rc,
        const Node::Port& in_port, VariableType type, int var_count, uint32_t& flags);
    static ShaderVariant Calc(const RenderContext& rc,
        const Node::Port& in_port, const ShaderVariant& expect, uint32_t& flags);

    static ShaderVariant DefaultValue(VariableType type, int count = 1);

}; // Evaluator

}