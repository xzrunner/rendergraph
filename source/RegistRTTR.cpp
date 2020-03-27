#include "rendergraph/node/input_nodes.h"
#include "rendergraph/node/math_nodes.h"
#include "rendergraph/node/value_nodes.h"

#define EXE_FILEPATH "rendergraph/node_include_gen.h"
#include "rendergraph/node_regist_cfg.h"
#undef EXE_FILEPATH

#include <rttr/registration>

#define REGIST_NODE_TYPE(type, name)                                            \
	rttr::registration::class_<rendergraph::device::type>("rendergraph::"#name) \
		.constructor<>()                                                        \
	;

#define REGIST_ENUM_ITEM(type, name, label) \
    rttr::value(name, type),                \
    rttr::metadata(type, label)             \

RTTR_REGISTRATION
{

rttr::registration::class_<dag::Node<rendergraph::Variable>::Port>("rendergraph::Node::Port")
	.property("var", &dag::Node<rendergraph::Variable>::Port::var)
;

rttr::registration::class_<rendergraph::Node>("rendergraph::Node")
	.method("GetImports", &rendergraph::Node::GetImports)
	.method("GetExports", &rendergraph::Node::GetExports)
;

#define EXE_FILEPATH "rendergraph/node_rttr_gen.h"
#include "rendergraph/node_regist_cfg.h"
#undef EXE_FILEPATH

//rttr::registration::enumeration<rendergraph::node::UserScript::ReturnType>("rg_user_script_return_type")
//(
//    REGIST_ENUM_ITEM(rendergraph::node::UserScript::ReturnType::Void,      "void",       "Void"),
//    REGIST_ENUM_ITEM(rendergraph::node::UserScript::ReturnType::Vec1Array, "vec1_array", "Vec1Array"),
//    REGIST_ENUM_ITEM(rendergraph::node::UserScript::ReturnType::Vec2Array, "vec2_array", "Vec2Array"),
//    REGIST_ENUM_ITEM(rendergraph::node::UserScript::ReturnType::Vec3Array, "vec3_array", "Vec3Array"),
//    REGIST_ENUM_ITEM(rendergraph::node::UserScript::ReturnType::Vec4Array, "vec4_array", "Vec4Array")
//);
//
//rttr::registration::enumeration<rendergraph::node::PrimitiveShape::Type>("rg_prim_shape_type")
//(
//    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::Type::Quad, "quad", "Quad"),
//    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::Type::Cube, "cube", "Cube")
//);
//
//rttr::registration::enumeration<rendergraph::node::PrimitiveShape::VertLayout>("rg_prim_shape_layout")
//(
//    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::VertLayout::Pos,          "pos",             "Pos"),
//    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::VertLayout::PosTex,       "pos_tex",         "PosTex"),
//    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::VertLayout::PosNormTex,   "pos_norm_tex",    "PosNormTex"),
//    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::VertLayout::PosNormTexTB, "pos_norm_tex_tb", "PosNormTexTB")
//);
//
//rttr::registration::enumeration<rendergraph::node::Texture::Type>("rg_texture_type")
//(
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Type::Tex2D,   "tex_2d",   "Tex2D"),
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Type::TexCube, "tex_cube", "TexCube")
//);
//
//rttr::registration::enumeration<rendergraph::node::Texture::Format>("rg_texture_format")
//(
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RGBA16, "rgba16", "RGBA16"),
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RGBA8,  "rgba8",  "RGBA8"),
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RGBA4,  "rgba4",  "RGBA4"),
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RGB,    "rgb",    "RGB"),
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RGB565, "rgb565", "RGB565"),
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::A8,     "a8",     "A8"),
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RED,    "red",    "RED"),
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::Depth,  "depth",  "Depth")
//);
//
//rttr::registration::enumeration<rendergraph::node::Texture::Wrapping>("rg_texture_wrapping")
//(
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Wrapping::Repeat,         "repeat",          "Repeat"),
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Wrapping::MirroredRepeat, "mirrored_repeat", "MirroredRepeat"),
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Wrapping::ClampToEdge,    "clamp_to_edge",   "ClampToEdge"),
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Wrapping::ClampToBorder,  "clamp_to_border", "ClampToBorder")
//);
//
//rttr::registration::enumeration<rendergraph::node::Texture::Filtering>("rg_texture_filtering")
//(
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Filtering::Nearest, "nearest", "Nearest"),
//    REGIST_ENUM_ITEM(rendergraph::node::Texture::Filtering::Linear,  "linear",  "Linear")
//);
//
//rttr::registration::enumeration<rendergraph::node::AlphaTest::Func>("rg_alpha_test_func")
//(
//    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::Off,      "off",      "Off"),
//    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::Never,    "never",    "Never"),
//    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::Less,     "less",     "Less"),
//    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::Equal,    "equal",    "Equal"),
//    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::LEqual,   "lequal",   "LEqual"),
//    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::Greater,  "greater",  "Greater"),
//    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::NotEqual, "notequal", "NotEqual"),
//    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::GEqual,   "gequal",   "GEqual"),
//    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::Always,   "always",   "Always")
//);

}

namespace rendergraph
{

void regist_rttr()
{
}

}