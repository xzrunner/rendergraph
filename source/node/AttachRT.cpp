#include "rendergraph/node/AttachRT.h"
#include "rendergraph/ValueImpl.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Framebuffer.h>

namespace rendergraph
{
namespace node
{

void AttachRT::Execute(const std::shared_ptr<dag::Context>& ctx)
{
	if (m_imports[I_RT].conns.empty() ||
		m_imports[I_TEX].conns.empty()) {
		return;
	}

	assert(m_imports[I_RT].conns.size() == 1 
		&& m_imports[I_TEX].conns.size() == 1);
	auto conn_rt = m_imports[I_RT].conns[0];
	auto conn_tex = m_imports[I_TEX].conns[0];

	auto rc = std::static_pointer_cast<RenderContext>(ctx);

	auto node_rt = conn_rt.node.lock();
	auto node_tex = conn_tex.node.lock();
	uint32_t flags = 0;
	ShaderVariant v_rt, v_tex;
	std::static_pointer_cast<Node>(node_rt)->Eval(*rc, conn_rt.idx, v_rt, flags);
	flags = 0;
	std::static_pointer_cast<Node>(node_tex)->Eval(*rc, conn_tex.idx, v_tex, flags);
	if (v_rt.type != VariableType::RenderTarget ||
		v_tex.type != VariableType::Texture ||
		!v_rt.p || !v_tex.p) {
		return;
	}

	const_cast<ur::Framebuffer*>(static_cast<const ur::Framebuffer*>(v_rt.p))->SetAttachment(
		static_cast<ur::AttachmentType>(m_attachment),
		static_cast<ur::TextureTarget>(m_textarget),
		static_cast<const TextureVal*>(v_tex.p)->texture,
		nullptr,
		m_mipmap_level
	);
}

}
}