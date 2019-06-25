#include "rendergraph/node/CustomExpression.h"
#include "rendergraph/node/ExpressionParser.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/ScriptEnv.h"

#include <cpputil/StringHelper.h>

#include <chaiscript/chaiscript.hpp>

namespace rg
{
namespace node
{

void CustomExpression::Execute(const RenderContext& rc)
{
    auto& chai = ScriptEnv::Instance()->GetChai();

    for (int i = I_MAX_NUM, n = m_imports.size(); i < n; ++i) 
    {
        auto& port = m_imports[i];
        if (port.conns.empty()) {
            continue;
        }

        if (port.var.type == VariableType::Shader)
        {
            auto shader = std::static_pointer_cast<node::Shader>(port.conns[0].node.lock());
            chai->add(chaiscript::var(std::ref(*shader)), port.var.name);
        }
        else if (port.var.type == VariableType::Vec3Array)
        {
            // todo
        }
    }

    try {
        chai->eval(m_body);
    } catch (const chaiscript::exception::eval_error &e) {
        printf("%s\n", m_body.c_str());
        std::cout << "ChaiScript Error\n" << e.pretty_print() << '\n';
    }
}

void CustomExpression::SetCode(const std::string& code)
{
    if (code.empty()) {
        return;
    }

    auto formated = code;
    cpputil::StringHelper::ReplaceAll(formated, "\\n", "\n");

    ExpressionParser parser(formated);
    parser.Parse();

    m_body = parser.GetBody();
    cpputil::StringHelper::ReplaceAll(m_body, "\\t", "    ");

    m_imports.erase(m_imports.begin() + 1, m_imports.end());
    m_exports.erase(m_exports.begin() + 1, m_exports.end());

    std::copy(parser.GetInputs().begin(), parser.GetInputs().end(), std::back_inserter(m_imports));
    std::copy(parser.GetOutputs().begin(), parser.GetOutputs().end(), std::back_inserter(m_exports));
}

}
}