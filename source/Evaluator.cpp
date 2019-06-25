#include "rendergraph/Evaluator.h"

namespace rg
{

ShaderVariant Evaluator::Calc(const RenderContext& rc, const Node::Port& in_port,
                              VariableType type, int var_count, uint32_t& flags)
{
    return Calc(rc, in_port, DefaultValue(type, var_count), flags);
}

ShaderVariant Evaluator::Calc(const RenderContext& rc, const Node::Port& in_port,
                              const ShaderVariant& expect, uint32_t& flags)
{
    auto& conns = in_port.conns;
    if (!conns.empty()) {
        auto node = conns[0].node.lock();
        if (node) {
            auto ret = expect;
            node->Eval(rc, conns[0].idx, ret, flags);
            return ret;
        }
    }
    return expect;
}

ShaderVariant Evaluator::DefaultValue(VariableType type, int count)
{
    ShaderVariant ret;

    if (count == 1)
    {
        switch (type)
        {
        case VariableType::Int:
            ret = ShaderVariant(0);
            break;
        case VariableType::Bool:
            ret = ShaderVariant(false);
            break;
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
    }
    else
    {
        assert(count > 1);
        switch (type)
        {
        case VariableType::Vector1:
            ret.type = VariableType::Vec1Array;
            break;
        case VariableType::Vector2:
            ret.type = VariableType::Vec2Array;
            break;
        case VariableType::Vector3:
            ret.type = VariableType::Vec3Array;
            break;
        case VariableType::Vector4:
            ret.type = VariableType::Vec4Array;
            break;
        default:
            assert(0);
        }
    }

    return ret;
}

}