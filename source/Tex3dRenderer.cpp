#include "rendergraph/Tex3dRenderer.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>
#include <painting3/Shader.h>
#include <painting3/Blackboard.h>
#include <shaderweaver/typedef.h>
#include <shaderweaver/Evaluator.h>
#include <shaderweaver/node/Uniform.h>
#include <shaderweaver/node/Input.h>
#include <shaderweaver/node/Output.h>
#include <shaderweaver/node/PositionTrans.h>
#include <shaderweaver/node/SampleTex3D.h>
#include <shaderweaver/node/Multiply.h>

namespace rg
{

Tex3dRenderer::Tex3dRenderer()
{
	InitShader();

	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	m_vbo = rc.CreateBuffer(ur::VERTEXBUFFER, nullptr, 0);
	m_ebo = rc.CreateBuffer(ur::INDEXBUFFER, nullptr, 0);
}

Tex3dRenderer::~Tex3dRenderer()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();
	rc.ReleaseBuffer(ur::VERTEXBUFFER, m_vbo);
	rc.ReleaseBuffer(ur::INDEXBUFFER, m_ebo);
}

void Tex3dRenderer::Flush()
{
	if (m_buf.indices.empty()) {
		return;
	}

	m_shader->Use();
	if (m_buf.indices.empty()) {
		return;
	}

	auto& rc = ur::Blackboard::Instance()->GetRenderContext();
	rc.BindTexture(m_tex_id, 0);
	if (m_buf.indices.empty()) {
		return;
	}

	PrepareRenderState();

	m_shader->SetMat4("u_model", sm::mat4().x);

	rc.BindBuffer(ur::VERTEXBUFFER, m_vbo);
	size_t vbuf_sz = sizeof(Vertex) * m_buf.vertices.size();
	rc.UpdateBuffer(m_vbo, m_buf.vertices.data(), vbuf_sz);

	rc.BindBuffer(ur::INDEXBUFFER, m_ebo);
	size_t ibuf_sz = sizeof(unsigned short) * m_buf.indices.size();
	rc.UpdateBuffer(m_ebo, m_buf.indices.data(), ibuf_sz);

	rc.DrawElements(ur::DRAW_TRIANGLES, 0, m_buf.indices.size());

	m_buf.Clear();

	RestoreRenderState();
}

void Tex3dRenderer::DrawCube(const float* positions, const float* texcoords, int texid, uint32_t color)
{
	if (m_tex_id != texid) {
		Flush();
		m_tex_id = texid;
	}

	m_buf.Reserve(6, 4);

	m_buf.index_ptr[0] = m_buf.curr_index;
	m_buf.index_ptr[1] = m_buf.curr_index + 1;
	m_buf.index_ptr[2] = m_buf.curr_index + 2;
	m_buf.index_ptr[3] = m_buf.curr_index;
	m_buf.index_ptr[4] = m_buf.curr_index + 2;
	m_buf.index_ptr[5] = m_buf.curr_index + 3;
	m_buf.index_ptr += 6;

	int ptr = 0;
	for (int i = 0; i < 4; ++i)
	{
		auto& v = m_buf.vert_ptr[i];
		v.pos.x = positions[ptr];
		v.pos.y = positions[ptr + 1];
		v.pos.z = positions[ptr + 2];
		v.uv.x = texcoords[ptr];
		v.uv.y = texcoords[ptr + 1];
		v.uv.z = texcoords[ptr + 2];
		v.col = color;
		ptr += 3;
	}
	m_buf.vert_ptr += 4;

	m_buf.curr_index += 4;
}

void Tex3dRenderer::InitShader()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	// layout
	std::vector<ur::VertexAttrib> layout;
	layout.push_back(ur::VertexAttrib("position", 3, sizeof(float),    28, 0));
	layout.push_back(ur::VertexAttrib("texcoord", 3, sizeof(float),    28, 12));
	layout.push_back(ur::VertexAttrib("color",    4, sizeof(uint8_t),  28, 24));
	auto layout_id = rc.CreateVertexLayout(layout);
	rc.BindVertexLayout(layout_id);

	// vert
	std::vector<sw::NodePtr> vert_nodes;

	auto projection = std::make_shared<sw::node::Uniform>("u_projection", sw::t_mat4);
	auto view       = std::make_shared<sw::node::Uniform>("u_view",       sw::t_mat4);
	auto model      = std::make_shared<sw::node::Uniform>("u_model",      sw::t_mat4);

	auto position   = std::make_shared<sw::node::Input>  ("position",     sw::t_pos3);

	auto pos_trans = std::make_shared<sw::node::PositionTrans>(3);
	sw::make_connecting({ projection, 0 }, { pos_trans, sw::node::PositionTrans::ID_PROJ });
	sw::make_connecting({ view,       0 }, { pos_trans, sw::node::PositionTrans::ID_VIEW });
	sw::make_connecting({ model,      0 }, { pos_trans, sw::node::PositionTrans::ID_MODEL });
	sw::make_connecting({ position,   0 }, { pos_trans, sw::node::PositionTrans::ID_POS });
	vert_nodes.push_back(pos_trans);

	// varying
	auto vert_in_uv  = std::make_shared<sw::node::Input>("texcoord", sw::t_uvw);
	auto vert_out_uv = std::make_shared<sw::node::Output>("v_texcoord", sw::t_uvw);
	sw::make_connecting({ vert_in_uv, 0 }, { vert_out_uv, 0 });
	vert_nodes.push_back(vert_out_uv);

	auto col_in_uv = std::make_shared<sw::node::Input>("color", sw::t_flt4);
	auto col_out_uv = std::make_shared<sw::node::Output>("v_color", sw::t_flt4);
	sw::make_connecting({ col_in_uv, 0 }, { col_out_uv, 0 });
	vert_nodes.push_back(col_out_uv);

	// frag
	auto tex_sample = std::make_shared<sw::node::SampleTex3D>();
	auto frag_in_tex = std::make_shared<sw::node::Uniform>("u_texture0", sw::t_tex3d);
	auto frag_in_uv = std::make_shared<sw::node::Input>("v_texcoord", sw::t_uvw);
	sw::make_connecting({ frag_in_tex, 0 }, { tex_sample, sw::node::SampleTex3D::ID_TEX });
	sw::make_connecting({ frag_in_uv,  0 }, { tex_sample, sw::node::SampleTex3D::ID_UV });

	auto mul = std::make_shared<sw::node::Multiply>();
	auto frag_in_col = std::make_shared<sw::node::Input>("v_color", sw::t_flt4);
	sw::make_connecting({ tex_sample, 0 }, { mul, sw::node::Multiply::ID_A});
	sw::make_connecting({ frag_in_col, 0 }, { mul, sw::node::Multiply::ID_B });

	// end
	sw::Evaluator vert(vert_nodes, sw::ST_VERT);
	sw::Evaluator frag({ mul }, sw::ST_FRAG);

	//printf("//////////////////////////////////////////////////////////////////////////\n");
	//printf("%s\n", vert.GetShaderStr().c_str());
	//printf("//////////////////////////////////////////////////////////////////////////\n");
	//printf("%s\n", frag.GetShaderStr().c_str());
	//printf("//////////////////////////////////////////////////////////////////////////\n");

	std::vector<std::string> texture_names;
	pt3::Shader::Params sp(texture_names, layout);
	sp.vs = vert.GetShaderStr().c_str();
	sp.fs = frag.GetShaderStr().c_str();

	sp.uniform_names.model_mat = "u_model";
	sp.uniform_names.view_mat  = "u_view";
	sp.uniform_names.proj_mat  = "u_projection";
	auto& wc = pt3::Blackboard::Instance()->GetWindowContext();
	m_shader = std::make_shared<pt3::Shader>(*wc, &rc, sp);
}

void Tex3dRenderer::PrepareRenderState()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	// enable alpha blend
	rc.EnableBlend(true);
	rc.SetBlend(ur::BLEND_SRC_ALPHA, ur::BLEND_ONE_MINUS_SRC_ALPHA);
	// enable alpha test
	rc.SetAlphaTest(ur::ALPHA_GREATER, 0.05f);
	// disable depth test
	rc.SetDepthTest(ur::DEPTH_DISABLE);
	// disable face cull
	rc.SetCull(ur::CULL_DISABLE);
}

void Tex3dRenderer::RestoreRenderState()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	// disable alpha blend
	rc.EnableBlend(false);
	// disable alpha test
	rc.SetAlphaTest(ur::ALPHA_DISABLE);
	// enable depth test
	rc.SetDepthTest(ur::DEPTH_LESS_EQUAL);
	rc.EnableDepthMask(true);
	// enable face cull
	rc.SetFrontFace(true);
	rc.SetCull(ur::CULL_BACK);
}

//////////////////////////////////////////////////////////////////////////
// struct Tex3dRenderer::Buffer
//////////////////////////////////////////////////////////////////////////

void Tex3dRenderer::Buffer::Reserve(size_t idx_count, size_t vtx_count)
{
	size_t sz = vertices.size();
	vertices.resize(sz + vtx_count);
	vert_ptr = vertices.data() + sz;

	sz = indices.size();
	indices.resize(sz + idx_count);
	index_ptr = indices.data() + sz;
}

void Tex3dRenderer::Buffer::Clear()
{
	vertices.resize(0);
	indices.resize(0);

	curr_index = 0;
	vert_ptr = nullptr;
	index_ptr = nullptr;
}

}