#include "rendergraph/node/DrawCube.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void DrawCube::Execute(ur::RenderContext& rc)
{
    rc.RenderCube();
}

}
}