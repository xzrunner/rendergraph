#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

RenderTarget::~RenderTarget()
{
    if (m_id != 0) {
        ur::Blackboard::Instance()->GetRenderContext().ReleaseRenderTarget(m_id);
    }
}

void RenderTarget::Execute(const RenderContext& rc)
{
    if (m_id != 0) {
        return;
    }

    m_id = rc.rc.CreateRenderTarget(m_id);
}

void RenderTarget::SetLastViewport(int x, int y, int w, int h)
{
    m_vp_x = x;
    m_vp_y = y;
    m_vp_w = w;
    m_vp_h = h;
}

void RenderTarget::GetLastViewport(int& x, int& y, int& w, int& h) const
{
    x = m_vp_x;
    y = m_vp_y;
    w = m_vp_w;
    h = m_vp_h;
}

}
}