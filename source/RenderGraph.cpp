#include "rendergraph/RenderGraph.h"

namespace rendergraph
{

CU_SINGLETON_DEFINITION(RenderGraph);

extern void regist_rttr();

RenderGraph::RenderGraph()
{
	regist_rttr();
}

}