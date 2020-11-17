#include "rendergraph/RenderGraph.h"

#include <shadergraph/ShaderGraph.h>

namespace rendergraph
{

CU_SINGLETON_DEFINITION(RenderGraph);

extern void regist_rttr();

RenderGraph::RenderGraph()
{
	regist_rttr();

	shadergraph::ShaderGraph::Instance();
}

}