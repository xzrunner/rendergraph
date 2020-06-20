#include "rendergraph/node/TextureCube.h"
#include "rendergraph/ValueImpl.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/node/Texture.h"

#include <unirender/Device.h>
#include <unirender/TextureDescription.h>

namespace rendergraph
{
namespace node
{

void TextureCube::Execute(const std::shared_ptr<dag::Context>& ctx)
{
	std::array<ur::TexturePtr, 6> textures;
	for (int i = 0; i < 6; ++i) 
	{
		if (m_imports[i].conns.empty()) {
			return;
		}
		assert(m_imports[i].conns.size() == 1);
		auto conn = m_imports[i].conns[0];
		auto prev_node = conn.node.lock();
		if (!prev_node || prev_node->get_type() != rttr::type::get<node::Texture>()) {
			return;
		}
		textures[i] = std::static_pointer_cast<node::Texture>(prev_node)->GetTexture();
	}

	auto rc = std::static_pointer_cast<RenderContext>(ctx);
	m_tex = rc->ur_dev->CreateTextureCubeMap(textures);
}

void TextureCube::Eval(const RenderContext& rc, size_t port_idx,
	                   ShaderVariant& var, uint32_t& flags) const
{
    if (port_idx == 0)
    {
        var.type = VariableType::Texture;

		auto tex = new TextureVal;
		tex->texture = m_tex;
		var.p = tex;
    }
}

}
}