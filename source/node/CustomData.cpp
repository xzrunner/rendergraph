#include "rendergraph/node/CustomData.h"
#include "rendergraph/ScriptEnv.h"
#include "rendergraph/RenderContext.h"

#include <chaiscript/chaiscript.hpp>

namespace rendergraph
{
namespace node
{

void CustomData::Eval(const RenderContext& rc, size_t port_idx, ShaderVariant& var, uint32_t& flags) const
{
    if (m_cached.type == VariableType::Any) {
        var = Run(rc, m_code, var.type);
        m_cached = var;
    } else {
        var = m_cached;
    }
}

const ShaderVariant& CustomData::GetCachedVar(const RenderContext& rc, VariableType type) const
{
    if (m_cached.type == VariableType::Any) {
        m_cached = Run(rc, m_code, type);
    }
    return m_cached;
}

ShaderVariant CustomData::Run(const RenderContext& rc, const std::string& code, VariableType type) const
{
	if (code.empty()) {
		return ShaderVariant();
	}

    ShaderVariant ret;
    ret.type = type;

    auto& chai = rc.script_env->GetChai();

    switch (type)
    {
    case VariableType::Vec1Array:
        try {
            ret.vec1_array = chai->eval<std::vector<float>>(code);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    case VariableType::Vec2Array:
        try {
            ret.vec2_array = chai->eval<std::vector<sm::vec2>>(code);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    case VariableType::Vec3Array:
        try {
            ret.vec3_array = chai->eval<std::vector<sm::vec3>>(code);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    case VariableType::Vec4Array:
        try {
            ret.vec4_array = chai->eval<std::vector<sm::vec4>>(code);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    }

    return ret;
}

}
}