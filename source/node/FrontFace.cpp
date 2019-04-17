#include "rendergraph/node/FrontFace.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void FrontFace::Execute(const RenderContext& rc)
{
    rc.rc.SetFrontFace(m_clockwise);
}

}
}