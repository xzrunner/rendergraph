#include "rendergraph/node/SetUniform.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/Evaluator.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void SetUniform::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (m_var_name.empty() || m_var_type == VariableType::Any) {
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
    auto expected = Evaluator::DefaultValue(m_var_type);
    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    auto val = Evaluator::Calc(*rc, m_imports[I_VALUE], expected, flags);
    shader->SetUniformValue(rc->ur_dev, m_var_name, val);
}

}
}