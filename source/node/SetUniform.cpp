#include "rendergraph/node/SetUniform.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/Evaluator.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/ValueImpl.h"

#include <unirender/Context.h>
#include <unirender/ShaderProgram.h>

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

	if (val.type == VariableType::Texture && val.p)
	{
		const int slot = shader->GetShader(*rc)->QueryTexSlot(m_var_name);
		shader->SetUniformValue(*rc, m_var_name, ShaderVariant(slot));
		auto tex = reinterpret_cast<const TextureVal*>(val.p);
		if (tex->texture) {
			rc->ur_ctx->SetTexture(slot, tex->texture);
		}
		if (tex->sampler) {
			rc->ur_ctx->SetTextureSampler(slot, tex->sampler);
		}
	}
	else
	{
		shader->SetUniformValue(*rc, m_var_name, val);
	}
}

}
}