#include "rendergraph/RenderMgr.h"
#include "rendergraph/IRenderer.h"
#include "rendergraph/SpriteRenderer.h"
#include "rendergraph/MeshRenderer.h"
#include "rendergraph/VolumeRenderer.h"
#include "rendergraph/ExternRenderer.h"

namespace rg
{

CU_SINGLETON_DEFINITION(RenderMgr);

RenderMgr::RenderMgr()
{
}

std::shared_ptr<IRenderer> RenderMgr::SetRenderer(RenderType type)
{
	if (m_curr_render != type)
	{
		if (m_curr_render > RenderType::NIL) {
			m_renderers[static_cast<int>(m_curr_render)]->Flush();
		}
		m_curr_render = type;
	}

	if (!m_renderers[static_cast<int>(m_curr_render)])
	{
		switch (m_curr_render)
		{
		case RenderType::NIL:
			m_renderers[static_cast<int>(RenderType::SPRITE)] = nullptr;
			break;
		case RenderType::SPRITE:
			m_renderers[static_cast<int>(RenderType::SPRITE)]
				= std::make_shared<SpriteRenderer>();
			break;
        case RenderType::MESH:
            m_renderers[static_cast<int>(RenderType::MESH)]
                = std::make_shared<MeshRenderer>();
            break;
		case RenderType::TEX3D:
			m_renderers[static_cast<int>(RenderType::TEX3D)]
				= std::make_shared<VolumeRenderer>();
			break;
		case RenderType::EXTERN:
			m_renderers[static_cast<int>(RenderType::EXTERN)]
				= std::make_shared<ExternRenderer>();
			break;
		}
	}
	return m_renderers[static_cast<int>(m_curr_render)];
}

bool RenderMgr::BindSprWndCtx(pt2::WindowContext& wc) const
{
    auto& rd = m_renderers[static_cast<int>(RenderType::SPRITE)];
    if (!rd) {
        return false;
    }

    std::static_pointer_cast<SpriteRenderer>(rd)->BindWindowContext(wc);
    return true;
}

bool RenderMgr::BindMeshWndCtx(pt3::WindowContext& wc) const
{
    auto& rd = m_renderers[static_cast<int>(RenderType::MESH)];
    if (!rd) {
        return false;
    }

    std::static_pointer_cast<MeshRenderer>(rd)->BindWindowContext(wc);
    return true;
}

void RenderMgr::Flush()
{
	auto shader = SetRenderer(m_curr_render);
	if (shader) {
		shader->Flush();
	}
}

}