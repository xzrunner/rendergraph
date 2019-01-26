#include "rendergraph/SpriteRenderer.h"
#include "rendergraph/Callback.h"
#include "rendergraph/RenderBuffer.h"

#include <SM_Rect.h>
#include <tessellation/Painter.h>
#include <tessellation/Palette.h>
#include <unirender/Blackboard.h>
#include <unirender/VertexAttrib.h>
#include <unirender/RenderContext.h>
#include <shaderweaver/typedef.h>
#include <shaderweaver/Evaluator.h>
#include <shaderweaver/node/Uniform.h>
#include <shaderweaver/node/Input.h>
#include <shaderweaver/node/Output.h>
#include <shaderweaver/node/PositionTrans.h>
#include <shaderweaver/node/SampleTex2D.h>
#include <shaderweaver/node/Multiply.h>
#include <painting2/Shader.h>

namespace
{

void copy_vertex_buffer(const sm::mat4& mat, rg::RenderBuffer<rg::SpriteVertex>& dst,
	                    const tess::Painter::Buffer& src)
{
	dst.Reserve(src.indices.size(), src.vertices.size());
	for (auto& i : src.indices) {
		*dst.index_ptr++ = dst.curr_index + i;
	}
	for (auto& v : src.vertices)
	{
		dst.vert_ptr->pos = mat * v.pos;
		dst.vert_ptr->uv  = v.uv;
		dst.vert_ptr->col = v.col;
		++dst.vert_ptr;
	}
	dst.curr_index += static_cast<unsigned short>(src.vertices.size());
}

}

namespace rg
{

SpriteRenderer::SpriteRenderer()
{
	InitShader();

	auto& rc = ur::Blackboard::Instance()->GetRenderContext();
	m_palette = std::make_unique<tess::Palette>(&rc);
}

void SpriteRenderer::Flush()
{
    FlushBuffer(ur::DRAW_TRIANGLES);
}

void SpriteRenderer::DrawQuad(const float* positions, const float* texcoords, int texid, uint32_t color)
{
	if (m_tex_id != texid) {
		Flush();
		m_tex_id = texid;
	}

    if (m_buf.vertices.size() + 4 >= RenderBuffer<SpriteVertex>::MAX_VERTEX_NUM) {
        Flush();
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
		v.uv.x = texcoords[ptr];
		v.uv.y = texcoords[ptr + 1];
		v.col = color;
		ptr += 2;
	}
	m_buf.vert_ptr += 4;

	m_buf.curr_index += 4;
}

void SpriteRenderer::DrawPainter(const tess::Painter& pt, const sm::mat4& mat)
{
	if (pt.IsEmpty()) {
		return;
	}

	if (m_palette->GetTexID() == m_tex_id)
	{
		copy_vertex_buffer(mat, m_buf, pt.GetBuffer());
	}
	else
	{
		int tex_id = m_palette->GetTexID();
		int tex_w = m_palette->GetTexWidth();
		int tex_h = m_palette->GetTexHeight();
		sm::irect qr(0, 0, tex_w, tex_h);
		int cached_texid;
		auto cached_texcoords = Callback::QueryCachedTexQuad(tex_id, qr, cached_texid);
		if (cached_texcoords)
		{
			if (cached_texid != m_tex_id)
			{
				Flush();
				m_tex_id = cached_texid;
			}

            if (m_buf.vertices.size() + pt.GetBuffer().vertices.size() >= RenderBuffer<SpriteVertex>::MAX_VERTEX_NUM) {
                Flush();
            }

			copy_vertex_buffer(mat, m_buf, pt.GetBuffer());

			float x = cached_texcoords[0];
			float y = cached_texcoords[1];
			float w = cached_texcoords[2] - cached_texcoords[0];
			float h = cached_texcoords[5] - cached_texcoords[1];
			size_t v_sz = pt.GetBuffer().vertices.size();

            assert(m_buf.curr_index - v_sz < m_buf.vertices.size());
			auto v_ptr = &m_buf.vertices[m_buf.curr_index - v_sz];
			for (size_t i = 0; i < v_sz; ++i)
			{
				auto& v = *v_ptr++;
				v.uv.x = x + w * v.uv.x;
				v.uv.y = y + h * v.uv.y;
			}
		}
		else
		{
			Callback::AddCacheSymbol(tex_id, tex_w, tex_h, qr);

			Flush();
			m_tex_id = m_palette->GetTexID();
			copy_vertex_buffer(mat, m_buf, pt.GetBuffer());
		}
	}
}

void SpriteRenderer::BindWindowContext(pt2::WindowContext& wc)
{
    std::static_pointer_cast<pt2::Shader>(m_shader)->AddNotify(wc);
}

void SpriteRenderer::InitShader()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	// layout
	std::vector<ur::VertexAttrib> layout;
	layout.push_back(ur::VertexAttrib("position", 2, sizeof(float),    20, 0));
	layout.push_back(ur::VertexAttrib("texcoord", 2, sizeof(float),    20, 8));
	layout.push_back(ur::VertexAttrib("color",    4, sizeof(uint8_t),  20, 16));
	auto layout_id = rc.CreateVertexLayout(layout);
	rc.BindVertexLayout(layout_id);

	// vert
	std::vector<sw::NodePtr> vert_nodes;

	auto projection = std::make_shared<sw::node::Uniform>("u_projection", sw::t_mat4);
	auto view       = std::make_shared<sw::node::Uniform>("u_view",       sw::t_mat4);
	auto model      = std::make_shared<sw::node::Uniform>("u_model",      sw::t_mat4);

	auto position   = std::make_shared<sw::node::Input>  ("position",     sw::t_pos2);

	auto pos_trans = std::make_shared<sw::node::PositionTrans>(2);
	sw::make_connecting({ projection, 0 }, { pos_trans, sw::node::PositionTrans::ID_PROJ });
	sw::make_connecting({ view,       0 }, { pos_trans, sw::node::PositionTrans::ID_VIEW });
	sw::make_connecting({ model,      0 }, { pos_trans, sw::node::PositionTrans::ID_MODEL });
	sw::make_connecting({ position,   0 }, { pos_trans, sw::node::PositionTrans::ID_POS });
	vert_nodes.push_back(pos_trans);

	// varying
	auto vert_in_uv  = std::make_shared<sw::node::Input>("texcoord", sw::t_uv);
	auto vert_out_uv = std::make_shared<sw::node::Output>("v_texcoord", sw::t_uv);
	sw::make_connecting({ vert_in_uv, 0 }, { vert_out_uv, 0 });
	vert_nodes.push_back(vert_out_uv);

	auto col_in_uv = std::make_shared<sw::node::Input>("color", sw::t_flt4);
	auto col_out_uv = std::make_shared<sw::node::Output>("v_color", sw::t_flt4);
	sw::make_connecting({ col_in_uv, 0 }, { col_out_uv, 0 });
	vert_nodes.push_back(col_out_uv);

	// frag
	auto tex_sample = std::make_shared<sw::node::SampleTex2D>();
	auto frag_in_tex = std::make_shared<sw::node::Uniform>("u_texture0", sw::t_tex2d);
	auto frag_in_uv = std::make_shared<sw::node::Input>("v_texcoord", sw::t_uv);
	sw::make_connecting({ frag_in_tex, 0 }, { tex_sample, sw::node::SampleTex2D::ID_TEX });
	sw::make_connecting({ frag_in_uv,  0 }, { tex_sample, sw::node::SampleTex2D::ID_UV });

	auto mul = std::make_shared<sw::node::Multiply>();
	auto frag_in_col = std::make_shared<sw::node::Input>("v_color", sw::t_flt4);
	sw::make_connecting({ tex_sample, 0 }, { mul, sw::node::Multiply::ID_A});
	sw::make_connecting({ frag_in_col, 0 }, { mul, sw::node::Multiply::ID_B });

	// end
	sw::Evaluator vert(vert_nodes, sw::ST_VERT);
	sw::Evaluator frag({ mul }, sw::ST_FRAG);

	//printf("//////////////////////////////////////////////////////////////////////////\n");
	//printf("%s\n", vert.GenShaderStr().c_str());
	//printf("//////////////////////////////////////////////////////////////////////////\n");
	//printf("%s\n", frag.GenShaderStr().c_str());
	//printf("//////////////////////////////////////////////////////////////////////////\n");

	std::vector<std::string> texture_names;
	pt2::Shader::Params sp(texture_names, layout);
	sp.vs = vert.GenShaderStr().c_str();
	sp.fs = frag.GenShaderStr().c_str();

	sp.uniform_names.model_mat = "u_model";
	sp.uniform_names.view_mat  = "u_view";
	sp.uniform_names.proj_mat  = "u_projection";

	m_shader = std::make_shared<pt2::Shader>(&rc, sp);
}

}