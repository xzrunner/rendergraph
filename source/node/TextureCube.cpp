#include "rendergraph/node/TextureCube.h"
#include "rendergraph/ValueImpl.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/Evaluator.h"
#include "rendergraph/node/Texture.h"

#include <unirender/Device.h>
#include <unirender/TextureDescription.h>

namespace rendergraph
{
namespace node
{

void TextureCube::Execute(const std::shared_ptr<dag::Context>& ctx)
{
	auto rc = std::static_pointer_cast<RenderContext>(ctx);
	BuildTexture(*rc);
}

void TextureCube::Eval(const RenderContext& rc, size_t port_idx,
	                   ShaderVariant& var, uint32_t& flags) const
{
    if (port_idx == 0)
    {
		if (!m_tex) {
			BuildTexture(rc);
		}

        var.type = VariableType::Texture;

		auto tex = new TextureVal;
		tex->texture = m_tex;
		var.p = tex;
    }
}

void TextureCube::BuildTexture(const RenderContext& rc) const
{
	std::array<ur::TexturePtr, 6> textures;
	for (int i = 0; i < 6; ++i)
	{
		if (m_imports[i].conns.empty()) {
			return;
		}

		uint32_t flags;
		auto var = Evaluator::Calc(rc, m_imports[i], Evaluator::DefaultValue(rendergraph::VariableType::Texture), flags);
		if (var.type != rendergraph::VariableType::Texture) {
			return;
		}

		textures[i] = var.p ? reinterpret_cast<const TextureVal*>(var.p)->texture : nullptr;
	}

	m_tex = rc.ur_dev->CreateTextureCubeMap(textures);
}

}
}