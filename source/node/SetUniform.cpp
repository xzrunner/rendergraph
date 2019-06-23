#include "rendergraph/node/SetUniform.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/Evaluator.h"

namespace rg
{
namespace node
{

void SetUniform::Execute(const RenderContext& rc)
{
    if (m_var_name.empty()) {
        return;
    }

    if (m_imports[I_SHADER].conns.empty()) {
        return;
    }

    auto shader_node = m_imports[I_SHADER].conns[0].node.lock();
    if (!shader_node ||
        shader_node->get_type() != rttr::type::get<Shader>()) {
        return;
    }

    auto shader = std::static_pointer_cast<Shader>(shader_node);

    uint32_t flags = 0;
    auto& in_port = m_imports[I_VALUE];
    auto expected = Evaluator::DefaultValue(in_port.var.type);
    auto val = Evaluator::Calc(rc, in_port, expected, flags);
    shader->SetUniformValue(m_var_name, val);
}

}
}