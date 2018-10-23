#include "rendergraph/RenderMgr.h"
#include "rendergraph/IRenderer.h"
#include "rendergraph/SpriteRenderer.h"
#include "rendergraph/ExternRenderer.h"

namespace rg
{

CU_SINGLETON_DEFINITION(RenderMgr);

RenderMgr::RenderMgr()
{
	m_renderers[static_cast<int>(RenderType::SPRITE)]
		= std::make_shared<SpriteRenderer>();
	m_renderers[static_cast<int>(RenderType::EXTERN)]
		= std::make_shared<ExternRenderer>();
}

std::shared_ptr<IRenderer> RenderMgr::SetRenderer(RenderType type)
{
	if (m_curr_render != static_cast<int>(type))
	{
		if (m_curr_render >= 0) {
			m_renderers[m_curr_render]->Flush();
		}
		m_curr_render = static_cast<int>(type);
	}
	return m_curr_render < 0 ? nullptr : m_renderers[m_curr_render];
}

void RenderMgr::Flush()
{
	if (m_curr_render >= 0) {
		m_renderers[m_curr_render]->Flush();
	}
}

}