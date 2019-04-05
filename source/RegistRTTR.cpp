#pragma once

#include "rendergraph/node/Clear.h"
#include "rendergraph/node/Viewport.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/node/Bind.h"
#include "rendergraph/node/Unbind.h"

#define REGIST_NODE_TYPE(name)                                \
	rttr::registration::class_<rg::node::name>("rg::"#name)   \
		.constructor<>()                                      \
	;

RTTR_REGISTRATION
{

REGIST_NODE_TYPE(Clear)
REGIST_NODE_TYPE(Viewport)
REGIST_NODE_TYPE(Texture)
REGIST_NODE_TYPE(RenderTarget)
REGIST_NODE_TYPE(Shader)
REGIST_NODE_TYPE(Bind)
REGIST_NODE_TYPE(Unbind)

}

namespace rg
{

void regist_rttr()
{
}

}