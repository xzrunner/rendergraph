#include "rendergraph/Evaluator.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/node/Input.h"
#include "rendergraph/node/Output.h"
#include "rendergraph/node/SubGraph.h"

namespace rendergraph
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
    if (conns.empty()) {
        return expect;
    }

    assert(conns.size() == 1);
    auto node = conns[0].node.lock();
    if (!node) {
        return expect;
    }

    if (node->get_type() == rttr::type::get<node::Input>() &&
        !rc.sub_graph_stack.empty())
    {
        auto input = std::static_pointer_cast<node::Input>(node);
        auto sub_graph = rc.sub_graph_stack.back();
        for (auto& in : sub_graph->GetImports())
		{
			if (in.var.type.name != input->GetVarName()) {
				continue;
			}
			rc.sub_graph_stack.pop_back();
            auto ret = Calc(rc, in, expect, flags);
			rc.sub_graph_stack.push_back(sub_graph);
			return ret;
        }
    }
    else if (node->get_type() == rttr::type::get<node::Output>() &&
        !rc.sub_graph_stack.empty())
    {
        int zz = 0;
    }

    auto ret = expect;
    std::static_pointer_cast<Node>(node)->Eval(rc, conns[0].idx, ret, flags);
    return ret;
}

ShaderVariant Evaluator::DefaultValue(VariableType type, int count)
{
    ShaderVariant ret;

    if (count == 1)
    {
        switch (type)
        {
        case VariableType::String:
            ret = ShaderVariant(std::string(""));
            break;
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
		case VariableType::Texture:
        case VariableType::Sampler2D:
        case VariableType::SamplerCube:
            ret = ShaderVariant(type, 0);
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