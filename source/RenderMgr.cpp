#include "rendergraph/RenderMgr.h"
#include "rendergraph/IRenderer.h"
#include "rendergraph/SpriteRenderer.h"
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
		case RenderType::EXTERN:
			m_renderers[static_cast<int>(RenderType::EXTERN)]
				= std::make_shared<ExternRenderer>();
			break;
		}
	}
	return m_renderers[static_cast<int>(m_curr_render)];
}

void RenderMgr::Flush()
{
	auto shader = SetRenderer(m_curr_render);
	if (shader) {
		shader->Flush();
	}
}

}