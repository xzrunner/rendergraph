#include "rendergraph/node/FrontFace.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rendergraph
{
namespace node
{

void FrontFace::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    std::static_pointer_cast<RenderContext>(ctx)->rc.SetFrontFace(m_clockwise);
}

}
}