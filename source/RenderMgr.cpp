#include "rendergraph/RenderMgr.h"
#include "rendergraph/IRenderer.h"
#include "rendergraph/SpriteRenderer.h"
#include "rendergraph/MeshRenderer.h"
#include "rendergraph/SkinRenderer.h"
#include "rendergraph/Shape3Renderer.h"
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
        case RenderType::SKIN:
            m_renderers[static_cast<int>(RenderType::SKIN)]
                = std::make_shared<SkinRenderer>();
            break;
        case RenderType::SHAPE3D:
            m_renderers[static_cast<int>(RenderType::SHAPE3D)]
                = std::make_shared<Shape3Renderer>();
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

bool RenderMgr::BindWndCtx2D(pt2::WindowContext& wc) const
{
    if (auto& sprite = m_renderers[static_cast<int>(RenderType::SPRITE)]) {
        std::static_pointer_cast<SpriteRenderer>(sprite)->BindWindowContext(wc);
    } else {
        return false;
    }
    return true;
}

bool RenderMgr::BindWndCtx3D(pt3::WindowContext& wc) const
{
    if (auto& shape = m_renderers[static_cast<int>(RenderType::SHAPE3D)]) {
        std::static_pointer_cast<Shape3Renderer>(shape)->BindWindowContext(wc);
    } else {
        return false;
    }
    if (auto& mesh = m_renderers[static_cast<int>(RenderType::MESH)]) {
        std::static_pointer_cast<MeshRenderer>(mesh)->BindWindowContext(wc);
    } else {
        return false;
    }
    if (auto& skin = m_renderers[static_cast<int>(RenderType::SKIN)]) {
        std::static_pointer_cast<SkinRenderer>(skin)->BindWindowContext(wc);
    } else {
        return false;
    }
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