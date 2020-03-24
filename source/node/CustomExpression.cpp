#include "rendergraph/node/CustomExpression.h"
#include "rendergraph/node/ExpressionParser.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/node/UserScript.h"
#include "rendergraph/ScriptEnv.h"
#include "rendergraph/Utility.h"

#include <chaiscript/chaiscript.hpp>

namespace rendergraph
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
            auto node = port.conns[0].node.lock();
            assert(node->get_type() == rttr::type::get<node::Shader>());
            auto shader = std::static_pointer_cast<node::Shader>(node);
            chai->add(chaiscript::var(shader), port.var.name);
        }
        else if (port.var.type == VariableType::Vec1Array)
        {
            auto node = port.conns[0].node.lock();
            assert(node->get_type() == rttr::type::get<node::UserScript>());
            auto& vec = std::static_pointer_cast<node::UserScript>(node)->GetCachedVar(port.var.type).vec1_array;
            chai->add(chaiscript::var(vec), port.var.name);
        }
        else if (port.var.type == VariableType::Vec2Array)
        {
            auto node = port.conns[0].node.lock();
            assert(node->get_type() == rttr::type::get<node::UserScript>());
            auto& vec = std::static_pointer_cast<node::UserScript>(node)->GetCachedVar(port.var.type).vec2_array;
            chai->add(chaiscript::var(vec), port.var.name);
        }
        else if (port.var.type == VariableType::Vec3Array)
        {
            auto node = port.conns[0].node.lock();
            assert(node->get_type() == rttr::type::get<node::UserScript>());
            auto& vec = std::static_pointer_cast<node::UserScript>(node)->GetCachedVar(port.var.type).vec3_array;

            // without:     m_chai->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<sm::vec3>>("Vec3Vector"));
            //std::vector<chaiscript::Boxed_Value> chai_vec;
            //chai_vec.reserve(vec.size());
            //for (auto& v : vec) {
            //    chai_vec.push_back(chaiscript::var(v));
            //}
            //chai->add(chaiscript::var(chai_vec), port.var.name);

            chai->add(chaiscript::var(vec), port.var.name);
        }
        else if (port.var.type == VariableType::Vec4Array)
        {
            auto node = port.conns[0].node.lock();
            assert(node->get_type() == rttr::type::get<node::UserScript>());
            auto& vec = std::static_pointer_cast<node::UserScript>(node)->GetCachedVar(port.var.type).vec4_array;
            chai->add(chaiscript::var(vec), port.var.name);
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

    ExpressionParser parser(Utility::FormatCode(code));
    parser.Parse();

    m_body = parser.GetBody();
    //cpputil::StringHelper::ReplaceAll(m_body, "\\t", "    ");

    m_imports.erase(m_imports.begin() + 1, m_imports.end());
    m_exports.erase(m_exports.begin() + 1, m_exports.end());

    std::copy(parser.GetInputs().begin(), parser.GetInputs().end(), std::back_inserter(m_imports));
    std::copy(parser.GetOutputs().begin(), parser.GetOutputs().end(), std::back_inserter(m_exports));
}

}
}