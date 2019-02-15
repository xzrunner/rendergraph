#include "rendergraph/BSPRenderer.h"
#include "rendergraph/UniformNames.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>
#include <unirender/VertexAttrib.h>
#include <painting3/Shader.h>
#include <shaderweaver/typedef.h>
#include <shaderweaver/Evaluator.h>
#include <shaderweaver/node/Uniform.h>
#include <shaderweaver/node/Input.h>
#include <shaderweaver/node/Output.h>
#include <shaderweaver/node/PositionTrans.h>
#include <shaderweaver/node/VertexShader.h>
#include <shaderweaver/node/FragmentShader.h>
#include <shaderweaver/node/SampleTex2D.h>
#include <shaderweaver/node/Multiply.h>
#include <shaderweaver/node/CameraPos.h>

namespace
{

const char* VERT_TEXCOORD_LIGHT_NAME = "texcoord_light";

}

namespace rg
{

BSPRenderer::BSPRenderer()
{
    InitShader();
}

void BSPRenderer::Draw() const
{
    m_shader->Use();
}

void BSPRenderer::InitShader()
{
    auto& rc = ur::Blackboard::Instance()->GetRenderContext();

    //////////////////////////////////////////////////////////////////////////
    // layout
    //////////////////////////////////////////////////////////////////////////

    std::vector<ur::VertexAttrib> layout;
    layout.emplace_back(VERT_POSITION_NAME,       3, 4, 28, 0);
    layout.emplace_back(VERT_TEXCOORD_NAME,       2, 4, 28, 12);
    layout.emplace_back(VERT_TEXCOORD_LIGHT_NAME, 2, 4, 28, 20);
    auto layout_id = rc.CreateVertexLayout(layout);
    rc.BindVertexLayout(layout_id);

    //////////////////////////////////////////////////////////////////////////
    // vert
    //////////////////////////////////////////////////////////////////////////

    std::vector<sw::NodePtr> vert_nodes;

	auto projection = std::make_shared<sw::node::Uniform>(PROJ_MAT_NAME,  sw::t_mat4);
	auto view       = std::make_shared<sw::node::Uniform>(VIEW_MAT_NAME,  sw::t_mat4);
	auto model      = std::make_shared<sw::node::Uniform>(MODEL_MAT_NAME, sw::t_mat4);

    auto position       = std::make_shared<sw::node::Input>(VERT_POSITION_NAME, sw::t_flt3);
    auto texcoord       = std::make_shared<sw::node::Input>(VERT_TEXCOORD_NAME, sw::t_uv);
    auto texcoord_light = std::make_shared<sw::node::Input>(VERT_TEXCOORD_LIGHT_NAME, sw::t_uv);

    // gl_Position =  u_projection * u_view * u_model * a_pos;
	auto pos_trans = std::make_shared<sw::node::PositionTrans>(3);
	sw::make_connecting({ projection, 0 }, { pos_trans, sw::node::PositionTrans::ID_PROJ });
	sw::make_connecting({ view, 0 },       { pos_trans, sw::node::PositionTrans::ID_VIEW });
	sw::make_connecting({ model, 0 },      { pos_trans, sw::node::PositionTrans::ID_MODEL });
	sw::make_connecting({ position, 0 },   { pos_trans, sw::node::PositionTrans::ID_POS });
    auto vert_end = std::make_shared<sw::node::VertexShader>();
    sw::make_connecting({ pos_trans, 0 }, { vert_end, 0 });

    // v_texcoord = a_texcoord;
    auto v_texcoord = std::make_shared<sw::node::Output>(FRAG_TEXCOORD_NAME, sw::t_uv);
    sw::make_connecting({ texcoord, 0 }, { v_texcoord, 0 });
    vert_nodes.push_back(v_texcoord);

    // v_texcoord_light = a_texcoord_light;
    auto v_texcoord_light = std::make_shared<sw::node::Output>(VERT_TEXCOORD_LIGHT_NAME, sw::t_uv);
    sw::make_connecting({ texcoord_light, 0 }, { v_texcoord_light, 0 });
    vert_nodes.push_back(v_texcoord_light);

    //////////////////////////////////////////////////////////////////////////
    // frag
    //////////////////////////////////////////////////////////////////////////

    // vec4 base = texture2D(u_base_tex, v_texcoord);
    auto base_tex_sample  = std::make_shared<sw::node::SampleTex2D>();
    auto frag_in_base_tex = std::make_shared<sw::node::Uniform>("u_base_tex", sw::t_tex2d);
    auto frag_in_base_uv  = std::make_shared<sw::node::Input>(FRAG_TEXCOORD_NAME, sw::t_uv);
    sw::make_connecting({ frag_in_base_tex, 0 }, { base_tex_sample, sw::node::SampleTex2D::ID_TEX });
    sw::make_connecting({ frag_in_base_uv,  0 }, { base_tex_sample, sw::node::SampleTex2D::ID_UV });

    // vec4 light = texture2D(u_light_tex, v_texcoord_light);
    auto light_tex_sample  = std::make_shared<sw::node::SampleTex2D>();
    auto frag_in_light_tex = std::make_shared<sw::node::Uniform>("u_light_tex", sw::t_tex2d);
    auto frag_in_light_uv  = std::make_shared<sw::node::Input>(FRAG_TEXCOORD_NAME, sw::t_uv);
    sw::make_connecting({ frag_in_light_tex, 0 }, { light_tex_sample, sw::node::SampleTex2D::ID_TEX });
    sw::make_connecting({ frag_in_light_uv,  0 }, { light_tex_sample, sw::node::SampleTex2D::ID_UV });

    // gl_FragColor = base * light;
    auto mul = std::make_shared<sw::node::Multiply>();
    sw::make_connecting({ base_tex_sample, 0 },  { mul, sw::node::Multiply::ID_A });
    sw::make_connecting({ light_tex_sample, 0 }, { mul, sw::node::Multiply::ID_B });

    auto frag_end = std::make_shared<sw::node::FragmentShader>();
    sw::make_connecting({ mul, 0 }, { frag_end, 0 });

    //////////////////////////////////////////////////////////////////////////
    // end
    //////////////////////////////////////////////////////////////////////////

	sw::Evaluator vert(vert_nodes);
	sw::Evaluator frag({ frag_end });

	//printf("//////////////////////////////////////////////////////////////////////////\n");
	//printf("%s\n", vert.GenShaderStr().c_str());
	//printf("//////////////////////////////////////////////////////////////////////////\n");
	//printf("%s\n", frag.GenShaderStr().c_str());
	//printf("//////////////////////////////////////////////////////////////////////////\n");

	std::vector<std::string> texture_names;
	pt3::Shader::Params sp(texture_names, layout);
	sp.vs = vert.GenShaderStr().c_str();
	sp.fs = frag.GenShaderStr().c_str();

	sp.uniform_names[pt0::U_MODEL_MAT] = MODEL_MAT_NAME;
	sp.uniform_names[pt0::U_VIEW_MAT]  = VIEW_MAT_NAME;
	sp.uniform_names[pt0::U_PROJ_MAT]  = PROJ_MAT_NAME;
    sp.uniform_names[pt0::U_CAM_POS]   = sw::node::CameraPos::CamPosName();

	m_shader = std::make_shared<pt3::Shader>(&rc, sp);
}

}