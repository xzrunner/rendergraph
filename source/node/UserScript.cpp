#include "rendergraph/node/UserScript.h"
#include "rendergraph/ScriptEnv.h"

#include <cpputil/StringHelper.h>

#include <chaiscript/chaiscript.hpp>

namespace rg
{
namespace node
{

void UserScript::Eval(const RenderContext& rc, size_t port_idx, ShaderVariant& var, uint32_t& flags) const
{
    if (m_cached.type == VariableType::Any) {
        var = Run(m_code, var.type);
        m_cached = var;
    } else {
        var = m_cached;
    }
}

const ShaderVariant& UserScript::GetCachedVar(VariableType type) const
{
    if (m_cached.type == VariableType::Any) {
        m_cached = Run(m_code, type);
    }
    return m_cached;
}

ShaderVariant UserScript::Run(const std::string& code, VariableType type)
{
    ShaderVariant ret;
    ret.type = type;

    auto formated = code;
    cpputil::StringHelper::ReplaceAll(formated, "\\n", "\n");
    cpputil::StringHelper::ReplaceAll(formated, "\\t", "\t");

    switch (type)
    {
    case VariableType::Vec1Array:
        try {
            ret.vec1_array = ScriptEnv::Instance()->GetChai()->eval<std::vector<float>>(formated);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    case VariableType::Vec2Array:
        try {
            ret.vec2_array = ScriptEnv::Instance()->GetChai()->eval<std::vector<sm::vec2>>(formated);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    case VariableType::Vec3Array:
        try {
            ret.vec3_array = ScriptEnv::Instance()->GetChai()->eval<std::vector<sm::vec3>>(formated);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    case VariableType::Vec4Array:
        try {
            ret.vec4_array = ScriptEnv::Instance()->GetChai()->eval<std::vector<sm::vec4>>(formated);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    }

    return ret;
}

}
}