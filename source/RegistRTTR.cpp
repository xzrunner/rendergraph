#pragma once

// resource
#include "rendergraph/node/Shader.h"
#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/VertexArray.h"
#include "rendergraph/node/PrimitiveShape.h"
// op
#include "rendergraph/node/Clear.h"
#include "rendergraph/node/Bind.h"
#include "rendergraph/node/Unbind.h"
#include "rendergraph/node/Draw.h"
// state
#include "rendergraph/node/Viewport.h"
#include "rendergraph/node/AlphaTest.h"
#include "rendergraph/node/BlendEq.h"
#include "rendergraph/node/BlendFunc.h"
#include "rendergraph/node/Cull.h"
#include "rendergraph/node/ZTest.h"
#include "rendergraph/node/ZWrite.h"
// others
#include "rendergraph/node/value_nodes.h"
#include "rendergraph/node/math_nodes.h"
#include "rendergraph/node/input_nodes.h"

#define REGIST_NODE_TYPE(name)                                \
	rttr::registration::class_<rg::node::name>("rg::"#name)   \
		.constructor<>()                                      \
	;

RTTR_REGISTRATION
{

// resource
REGIST_NODE_TYPE(Shader)
REGIST_NODE_TYPE(RenderTarget)
REGIST_NODE_TYPE(Texture)
REGIST_NODE_TYPE(VertexArray)
REGIST_NODE_TYPE(PrimitiveShape)
// op
REGIST_NODE_TYPE(Clear)
REGIST_NODE_TYPE(Bind)
REGIST_NODE_TYPE(Unbind)
REGIST_NODE_TYPE(Draw)
// state
REGIST_NODE_TYPE(Viewport)
REGIST_NODE_TYPE(AlphaTest)
REGIST_NODE_TYPE(BlendEq)
REGIST_NODE_TYPE(BlendFunc)
REGIST_NODE_TYPE(Cull)
REGIST_NODE_TYPE(ZTest)
REGIST_NODE_TYPE(ZWrite)
// value
REGIST_NODE_TYPE(Vector1)
REGIST_NODE_TYPE(Vector2)
REGIST_NODE_TYPE(Vector3)
REGIST_NODE_TYPE(Vector4)
REGIST_NODE_TYPE(Matrix2)
REGIST_NODE_TYPE(Matrix3)
REGIST_NODE_TYPE(Matrix4)
// math
REGIST_NODE_TYPE(Add)
REGIST_NODE_TYPE(Mul)
REGIST_NODE_TYPE(PerspectiveMat)
REGIST_NODE_TYPE(OrthoMat)
REGIST_NODE_TYPE(LookAtMat)
// input
REGIST_NODE_TYPE(CamProjMat)
REGIST_NODE_TYPE(CamViewMat)
REGIST_NODE_TYPE(CameraPosition)
REGIST_NODE_TYPE(LightPosition)

}

namespace rg
{

void regist_rttr()
{
}

}