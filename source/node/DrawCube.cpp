#include "rendergraph/node/DrawCube.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void DrawCube::Execute(const RenderContext& rc)
{
    rc.rc.RenderCube();
}

}
}