// resource
#include "rendergraph/node/Shader.h"
#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/VertexArray.h"
#include "rendergraph/node/PrimitiveShape.h"
#include "rendergraph/node/Model.h"
// op
#include "rendergraph/node/Clear.h"
#include "rendergraph/node/Bind.h"
#include "rendergraph/node/Unbind.h"
#include "rendergraph/node/Draw.h"
#include "rendergraph/node/SetUniform.h"
// state
#include "rendergraph/node/Viewport.h"
#include "rendergraph/node/AlphaTest.h"
#include "rendergraph/node/BlendEq.h"
#include "rendergraph/node/BlendFunc.h"
#include "rendergraph/node/Cull.h"
#include "rendergraph/node/ZTest.h"
#include "rendergraph/node/ZWrite.h"
#include "rendergraph/node/FrontFace.h"
// others
#include "rendergraph/node/value_nodes.h"
#include "rendergraph/node/math_nodes.h"
#include "rendergraph/node/input_nodes.h"
#include "rendergraph/node/UserScript.h"
#include "rendergraph/node/ForEachLoop.h"

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
REGIST_NODE_TYPE(Model)
// op
REGIST_NODE_TYPE(Clear)
REGIST_NODE_TYPE(Bind)
REGIST_NODE_TYPE(Unbind)
REGIST_NODE_TYPE(Draw)
REGIST_NODE_TYPE(SetUniform)
// state
REGIST_NODE_TYPE(Viewport)
REGIST_NODE_TYPE(AlphaTest)
REGIST_NODE_TYPE(BlendEq)
REGIST_NODE_TYPE(BlendFunc)
REGIST_NODE_TYPE(Cull)
REGIST_NODE_TYPE(ZTest)
REGIST_NODE_TYPE(ZWrite)
REGIST_NODE_TYPE(FrontFace)
// value
REGIST_NODE_TYPE(Int)
REGIST_NODE_TYPE(Bool)
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
REGIST_NODE_TYPE(Translate)
REGIST_NODE_TYPE(Rotate)
REGIST_NODE_TYPE(Scale)
// input
REGIST_NODE_TYPE(ProjectMat)
REGIST_NODE_TYPE(ViewMat)
REGIST_NODE_TYPE(ModelMat)
REGIST_NODE_TYPE(CameraPosition)
REGIST_NODE_TYPE(LightPosition)
REGIST_NODE_TYPE(UserScript)
// tool
REGIST_NODE_TYPE(ForEachLoop)

}

namespace rg
{

void regist_rttr()
{
}

}