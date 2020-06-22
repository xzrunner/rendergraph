#include "rendergraph/node/UserScript.h"
#include "rendergraph/ScriptEnv.h"
#include "rendergraph/Utility.h"
#include "rendergraph/RenderContext.h"

#include <chaiscript/chaiscript.hpp>

namespace rendergraph
{
namespace node
{

void UserScript::Eval(const RenderContext& rc, size_t port_idx, ShaderVariant& var, uint32_t& flags) const
{
    if (m_cached.type == VariableType::Any) {
        var = Run(rc, m_code, var.type);
        m_cached = var;
    } else {
        var = m_cached;
    }
}

const ShaderVariant& UserScript::GetCachedVar(const RenderContext& rc, VariableType type) const
{
    if (m_cached.type == VariableType::Any) {
        m_cached = Run(rc, m_code, type);
    }
    return m_cached;
}

ShaderVariant UserScript::Run(const RenderContext& rc, const std::string& code, VariableType type) const
{
	if (code.empty()) {
		return ShaderVariant();
	}

    ShaderVariant ret;
    ret.type = type;

    auto& chai = rc.script_env->GetChai();

    auto formated = Utility::FormatCode(code);
    switch (type)
    {
    case VariableType::Vec1Array:
        try {
            ret.vec1_array = chai->eval<std::vector<float>>(formated);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    case VariableType::Vec2Array:
        try {
            ret.vec2_array = chai->eval<std::vector<sm::vec2>>(formated);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    case VariableType::Vec3Array:
        try {
            ret.vec3_array = chai->eval<std::vector<sm::vec3>>(formated);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    case VariableType::Vec4Array:
        try {
            ret.vec4_array = chai->eval<std::vector<sm::vec4>>(formated);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    }

    return ret;
}

}
}