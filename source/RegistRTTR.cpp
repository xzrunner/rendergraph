#pragma once

#include "rendergraph/node/Clear.h"
#include "rendergraph/node/Viewport.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/node/Bind.h"
#include "rendergraph/node/Unbind.h"
#include "rendergraph/node/DrawCube.h"
#include "rendergraph/node/Value.h"

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
REGIST_NODE_TYPE(DrawCube)

// value
REGIST_NODE_TYPE(Vector1)
REGIST_NODE_TYPE(Vector2)
REGIST_NODE_TYPE(Vector3)
REGIST_NODE_TYPE(Vector4)
REGIST_NODE_TYPE(Matrix2)
REGIST_NODE_TYPE(Matrix3)
REGIST_NODE_TYPE(Matrix4)
REGIST_NODE_TYPE(Add)
REGIST_NODE_TYPE(Mul)
REGIST_NODE_TYPE(PerspectiveMat)
REGIST_NODE_TYPE(OrthoMat)
REGIST_NODE_TYPE(LookAtMat)

}

namespace rg
{

void regist_rttr()
{
}

}