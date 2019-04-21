#include "rendergraph/Evaluator.h"

namespace rg
{

ShaderVariant Evaluator::Calc(const RenderContext& rc, const Node::Port& in_port,
                              VariableType type, uint32_t& flags)
{
    return Calc(rc, in_port, DefaultValue(type), flags);
}

ShaderVariant Evaluator::Calc(const RenderContext& rc, const Node::Port& in_port,
                              ShaderVariant expect, uint32_t& flags)
{
    auto& conns = in_port.conns;
    if (!conns.empty()) {
        auto node = conns[0].node.lock();
        if (node) {
            node->Eval(rc, conns[0].idx, expect, flags);
        }
    }
    return expect;
}

ShaderVariant Evaluator::DefaultValue(VariableType type)
{
    ShaderVariant ret;
    switch (type)
    {
    case VariableType::Vector1:
        ret = ShaderVariant(0.0f);
        break;
    case VariableType::Vector2:
        ret = ShaderVariant(sm::vec2());
        break;
    case VariableType::Vector3:
        ret = ShaderVariant(sm::vec3());
        break;
    case VariableType::Vector4:
        ret = ShaderVariant(sm::vec4());
        break;
    case VariableType::Matrix2:
        ret = ShaderVariant(sm::mat2());
        break;
    case VariableType::Matrix3:
        ret = ShaderVariant(sm::mat3());
        break;
    case VariableType::Matrix4:
        ret = ShaderVariant(sm::mat4());
        break;
    case VariableType::Sampler2D:
        ret = ShaderVariant(VariableType::Sampler2D, 0);
        break;
    case VariableType::SamplerCube:
        ret = ShaderVariant(VariableType::SamplerCube, 0);
        break;
    default:
        assert(0);
    }
    return ret;
}

}