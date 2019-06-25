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
    auto code = m_code;
    cpputil::StringHelper::ReplaceAll(code, "\\n", "\n");
    cpputil::StringHelper::ReplaceAll(code, "\\t", "\t");

    switch (var.type)
    {
    case VariableType::Vec1Array:
        try {
            var.vec1_array = ScriptEnv::Instance()->GetChai()->eval<std::vector<float>>(code);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    case VariableType::Vec2Array:
        try {
            var.vec2_array = ScriptEnv::Instance()->GetChai()->eval<std::vector<sm::vec2>>(code);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    case VariableType::Vec3Array:
        try {
            var.vec3_array = ScriptEnv::Instance()->GetChai()->eval<std::vector<sm::vec3>>(code);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    case VariableType::Vec4Array:
        try {
            var.vec4_array = ScriptEnv::Instance()->GetChai()->eval<std::vector<sm::vec4>>(code);
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
        }
        break;
    }
}

}
}