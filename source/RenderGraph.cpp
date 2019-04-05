#include "rendergraph/RenderGraph.h"

namespace rg
{

CU_SINGLETON_DEFINITION(RenderGraph);

extern void regist_rttr();

RenderGraph::RenderGraph()
{
	regist_rttr();
}

}