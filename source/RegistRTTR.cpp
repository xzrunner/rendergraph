#include "rendergraph/node/input_nodes.h"
#include "rendergraph/node/math_nodes.h"
#include "rendergraph/node/value_nodes.h"
#include "rendergraph/node/ShaderParser.h"
#include "rendergraph/node/Clear.h"
#include "rendergraph/node/AttachRT.h"
#include "rendergraph/node/String.h"

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

rttr::registration::enumeration<rendergraph::VariableType>("rg_var_type")
(
    REGIST_ENUM_ITEM(rendergraph::VariableType::Any,          "any",           "Any"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Port,         "port",          "Port"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Texture,      "texture",       "Texture"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::RenderTarget, "render_target", "RenderTarget"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Shader,       "shader",        "Shader"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Model,        "model",         "Model"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::String,       "string",        "String"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Int,          "int",           "Int"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Bool,         "bool",          "Bool"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Vector1,      "vec1",          "Vector1"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Vector2,      "vec2",          "Vector2"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Vector3,      "vec3",          "Vector3"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Vector4,      "vec4",          "Vector4"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Matrix2,      "mat2",          "Matrix2"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Matrix3,      "mat3",          "Matrix3"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Matrix4,      "mat4",          "Matrix4"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Sampler2D,    "sampler_2d",    "Sampler2D"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::SamplerCube,  "sampler_cube",  "SamplerCube"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Vec1Array,    "vec1_array",    "Vec1Array"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Vec2Array,    "vec2_array",    "Vec2Array"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Vec3Array,    "vec3_array",    "Vec3Array"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::Vec4Array,    "vec4_array",    "Vec4Array"),
    REGIST_ENUM_ITEM(rendergraph::VariableType::UserType,     "user_type",     "UserType")
);

rttr::registration::enumeration<rendergraph::node::ShaderKeyword>("rg_shader_keyword")
(
    REGIST_ENUM_ITEM(rendergraph::node::ShaderKeyword::Struct,  "struct",  "Struct"),
    REGIST_ENUM_ITEM(rendergraph::node::ShaderKeyword::Uniform, "uniform", "Uniform")
);

rttr::registration::enumeration<rendergraph::node::Clear::Type>("rg_clear_type")
(
    REGIST_ENUM_ITEM(rendergraph::node::Clear::Type::Color,   "color",   "Color"),
    REGIST_ENUM_ITEM(rendergraph::node::Clear::Type::Depth,   "depth",   "Depth"),
    REGIST_ENUM_ITEM(rendergraph::node::Clear::Type::Stencil, "stencil", "Stencil")
);

rttr::registration::enumeration<rendergraph::node::CustomData::ReturnType>("rg_user_script_return_type")
(
    REGIST_ENUM_ITEM(rendergraph::node::CustomData::ReturnType::Void,      "void",       "Void"),
    REGIST_ENUM_ITEM(rendergraph::node::CustomData::ReturnType::Vec1Array, "vec1_array", "Vec1Array"),
    REGIST_ENUM_ITEM(rendergraph::node::CustomData::ReturnType::Vec2Array, "vec2_array", "Vec2Array"),
    REGIST_ENUM_ITEM(rendergraph::node::CustomData::ReturnType::Vec3Array, "vec3_array", "Vec3Array"),
    REGIST_ENUM_ITEM(rendergraph::node::CustomData::ReturnType::Vec4Array, "vec4_array", "Vec4Array")
);

rttr::registration::enumeration<rendergraph::node::PrimitiveShape::Type>("rg_prim_shape_type")
(
    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::Type::Quad,   "quad",   "Quad"),
    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::Type::Cube,   "cube",   "Cube"),
    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::Type::Sphere, "sphere", "Sphere"),
    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::Type::Grids,  "grids",  "Grids")
);

rttr::registration::enumeration<rendergraph::node::PrimitiveShape::VertLayout>("rg_prim_shape_layout")
(
    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::VertLayout::Pos,          "pos",             "Pos"),
    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::VertLayout::PosTex,       "pos_tex",         "PosTex"),
    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::VertLayout::PosNorm,      "pos_norm",        "PosNorm"),
    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::VertLayout::PosNormTex,   "pos_norm_tex",    "PosNormTex"),
    REGIST_ENUM_ITEM(rendergraph::node::PrimitiveShape::VertLayout::PosNormTexTB, "pos_norm_tex_tb", "PosNormTexTB")
);

rttr::registration::enumeration<rendergraph::node::Texture::Type>("rg_texture_type")
(
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Type::Tex2D,   "2d",   "Tex2D"),
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Type::TexCube, "cube", "TexCube")
);

rttr::registration::enumeration<rendergraph::node::Texture::Format>("rg_texture_format")
(
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RGBA16, "rgba16", "RGBA16"),
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RGBA8,  "rgba8",  "RGBA8"),
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RGBA4,  "rgba4",  "RGBA4"),
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RGB,    "rgb",    "RGB"),
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RGB565, "rgb565", "RGB565"),
	REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RGB16F, "rgb16f", "RGB16F"),
	REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RG16F,  "rg16f",  "RG16F"),
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::A8,     "a8",     "A8"),
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::RED,    "red",    "RED"),
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Format::Depth,  "depth",  "Depth")
);

rttr::registration::enumeration<rendergraph::node::Texture::Wrapping>("rg_texture_wrapping")
(
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Wrapping::Repeat,         "repeat",          "Repeat"),
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Wrapping::MirroredRepeat, "mirrored_repeat", "MirroredRepeat"),
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Wrapping::ClampToEdge,    "clamp_to_edge",   "ClampToEdge"),
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Wrapping::ClampToBorder,  "clamp_to_border", "ClampToBorder")
);

rttr::registration::enumeration<rendergraph::node::Texture::Filtering>("rg_texture_filtering")
(
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Filtering::Nearest, "nearest", "Nearest"),
    REGIST_ENUM_ITEM(rendergraph::node::Texture::Filtering::Linear,  "linear",  "Linear")
);

rttr::registration::enumeration<rendergraph::node::AlphaTest::Func>("rg_alpha_test_func")
(
    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::Off,      "off",       "Off"),
    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::Never,    "never",     "Never"),
    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::Less,     "less",      "Less"),
    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::Equal,    "equal",     "Equal"),
    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::LEqual,   "lequal",    "LEqual"),
    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::Greater,  "greater",   "Greater"),
    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::NotEqual, "not_equal", "NotEqual"),
    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::GEqual,   "gequal",    "GEqual"),
    REGIST_ENUM_ITEM(rendergraph::node::AlphaTest::Func::Always,   "always",    "Always")
);

rttr::registration::enumeration<rendergraph::node::BlendEq::Mode>("rg_blendeq_mode")
(
    REGIST_ENUM_ITEM(rendergraph::node::BlendEq::Mode::FuncAdd,             "func_add",         "FuncAdd"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendEq::Mode::FuncSubtract,        "func_sub",         "FuncSubtract"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendEq::Mode::FuncReverseSubtract, "func_reverse_sub", "FuncReverseSubtract"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendEq::Mode::Min,                 "min",              "Min"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendEq::Mode::Max,                 "max",              "Max")
);

rttr::registration::enumeration<rendergraph::node::BlendFunc::Factor>("rg_blend_func_factor")
(
    REGIST_ENUM_ITEM(rendergraph::node::BlendFunc::Factor::Off,              "off",                 "Off"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendFunc::Factor::Zero,             "zero",                "Zero"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendFunc::Factor::One,              "one",                 "One"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendFunc::Factor::SrcColor,         "src_color",           "SrcColor"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendFunc::Factor::OneMinusSrcColor, "one_minus_src_color", "OneMinusSrcColor"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendFunc::Factor::DstColor,         "dst_color",           "DstColor"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendFunc::Factor::OneMinusDstColor, "one_minus_dst_color", "OneMinusDstColor"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendFunc::Factor::SrcAlpha,         "src_alpha",           "SrcAlpha"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendFunc::Factor::OneMinusSrcAlpha, "one_minus_src_alpha", "OneMinusSrcAlpha"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendFunc::Factor::DstAlpha,         "dst_alpha",           "DstAlpha"),
    REGIST_ENUM_ITEM(rendergraph::node::BlendFunc::Factor::OneMinusDstAlpha, "one_minus_dst_alpha", "OneMinusDstAlpha")
);

rttr::registration::enumeration<rendergraph::node::Cull::Mode>("rg_cull_mode")
(
    REGIST_ENUM_ITEM(rendergraph::node::Cull::Mode::Off,          "off",            "Off"),
    REGIST_ENUM_ITEM(rendergraph::node::Cull::Mode::Front,        "front",          "Front"),
    REGIST_ENUM_ITEM(rendergraph::node::Cull::Mode::Back,         "back",           "Back"),
    REGIST_ENUM_ITEM(rendergraph::node::Cull::Mode::FrontAndBack, "front_and_back", "FrontAndBack")
);

rttr::registration::enumeration<rendergraph::node::Rasterization::Mode>("rg_raster_mode")
(
    REGIST_ENUM_ITEM(rendergraph::node::Rasterization::Mode::Point, "point", "Point"),
    REGIST_ENUM_ITEM(rendergraph::node::Rasterization::Mode::Line,  "line",  "Line"),
    REGIST_ENUM_ITEM(rendergraph::node::Rasterization::Mode::Fill,  "fill",  "Fill")
);

rttr::registration::enumeration<rendergraph::node::ZTest::Func>("rg_ztest_func")
(
    REGIST_ENUM_ITEM(rendergraph::node::ZTest::Func::Off,      "off",       "Off"),
    REGIST_ENUM_ITEM(rendergraph::node::ZTest::Func::Never,    "never",     "Never"),
    REGIST_ENUM_ITEM(rendergraph::node::ZTest::Func::Less,     "less",      "Less"),
    REGIST_ENUM_ITEM(rendergraph::node::ZTest::Func::Equal,    "equal",     "Equal"),
    REGIST_ENUM_ITEM(rendergraph::node::ZTest::Func::LEqual,   "lequal",    "LEqual"),
    REGIST_ENUM_ITEM(rendergraph::node::ZTest::Func::Greater,  "greater",   "Greater"),
    REGIST_ENUM_ITEM(rendergraph::node::ZTest::Func::NotEqual, "not_equal", "NotEqual"),
    REGIST_ENUM_ITEM(rendergraph::node::ZTest::Func::GEqual,   "gequal",    "GEqual"),
    REGIST_ENUM_ITEM(rendergraph::node::ZTest::Func::Always,   "always",    "Always")
);

rttr::registration::enumeration<rendergraph::node::AttachRT::AttachmentType>("rg_attachrt_attachment")
(
    REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color0,  "color0",  "Color0"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color1,  "color1",  "Color1"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color2,  "color2",  "Color2"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color3,  "color3",  "Color3"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color4,  "color4",  "Color4"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color5,  "color5",  "Color5"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color6,  "color6",  "Color6"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color7,  "color7",  "Color7"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color8,  "color8",  "Color8"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color9,  "color9",  "Color9"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color10, "color10", "Color10"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color11, "color11", "Color11"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color12, "color12", "Color12"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color13, "color13", "Color13"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color14, "color14", "Color14"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Color15, "color15", "Color15"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Depth,   "depth",   "Depth"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::AttachmentType::Stencil, "stencil", "Stencil")
);

rttr::registration::enumeration<rendergraph::node::AttachRT::TextureTarget>("rg_attachrt_textarget")
(
    REGIST_ENUM_ITEM(rendergraph::node::AttachRT::TextureTarget::Texture1D,       "tex1d",       "Texture1D"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::TextureTarget::Texture2D,       "tex2d",       "Texture2D"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::TextureTarget::Texture3D,       "tex3d",       "Texture3D"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::TextureTarget::TextureCubeMap,  "texcubemap",  "TextureCubeMap"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::TextureTarget::TextureCubeMap0, "texcubemap0", "TextureCubeMap0"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::TextureTarget::TextureCubeMap1, "texcubemap1", "TextureCubeMap1"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::TextureTarget::TextureCubeMap2, "texcubemap2", "TextureCubeMap2"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::TextureTarget::TextureCubeMap3, "texcubemap3", "TextureCubeMap3"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::TextureTarget::TextureCubeMap4, "texcubemap4", "TextureCubeMap4"),
	REGIST_ENUM_ITEM(rendergraph::node::AttachRT::TextureTarget::TextureCubeMap5, "texcubemap5", "TextureCubeMap5")
);

rttr::registration::enumeration<rendergraph::node::Shader::Language>("rg_shader_lang")
(
    REGIST_ENUM_ITEM(rendergraph::node::Shader::Language::GLSL, "glsl", "GLSL"),
    REGIST_ENUM_ITEM(rendergraph::node::Shader::Language::HLSL, "hlsl", "HLSL")
);

rttr::registration::enumeration<rendergraph::node::String::Type>("rg_string_type")
(
    REGIST_ENUM_ITEM(rendergraph::node::String::Type::Data, "data", "Data"),
    REGIST_ENUM_ITEM(rendergraph::node::String::Type::Code, "code", "Code")
);

rttr::registration::enumeration<rendergraph::node::ShaderGraph::VertexShader>("rg_shadergraph_vs")
(
    REGIST_ENUM_ITEM(rendergraph::node::ShaderGraph::VertexShader::Image, "image", "Image"),
    REGIST_ENUM_ITEM(rendergraph::node::ShaderGraph::VertexShader::Model, "model", "Model")
);

}

namespace rendergraph
{

void regist_rttr()
{
}

}