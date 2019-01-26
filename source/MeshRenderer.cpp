#include "rendergraph/MeshRenderer.h"

#include <unirender/Blackboard.h>
#include <unirender/VertexAttrib.h>
#include <unirender/RenderContext.h>
#include <painting3/Shader.h>
#include <shaderweaver/typedef.h>
#include <shaderweaver/Evaluator.h>
#include <shaderweaver/node/Uniform.h>
#include <shaderweaver/node/Input.h>
#include <shaderweaver/node/Output.h>
#include <shaderweaver/node/PositionTrans.h>
#include <shaderweaver/node/Multiply.h>

namespace rg
{

MeshRenderer::MeshRenderer()
{
    InitShader();
}

void MeshRenderer::Flush()
{
    FlushBuffer(m_draw_mode);
}

void MeshRenderer::DrawLines(size_t num, const float* positions, uint32_t color)
{
    if (m_draw_mode != ur::DRAW_LINES) {
        Flush();
        m_draw_mode = ur::DRAW_LINES;
    }

    if (m_buf.vertices.size() + num >= RenderBuffer<MeshVertex>::MAX_VERTEX_NUM) {
        Flush();
    }

    m_buf.Reserve(num, num);

    int ptr = 0;
    for (size_t i = 0; i < num; ++i)
    {
        auto& v = m_buf.vert_ptr[i];
        v.pos.x = positions[ptr++];
        v.pos.y = positions[ptr++];
        v.pos.z = positions[ptr++];
        v.col = color;

        *m_buf.index_ptr++ = m_buf.curr_index + i;
    }
    m_buf.curr_index += num;
}

void MeshRenderer::BindWindowContext(pt3::WindowContext& wc)
{
    std::static_pointer_cast<pt3::Shader>(m_shader)->AddNotify(wc);
}

void MeshRenderer::InitShader()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	// layout
	std::vector<ur::VertexAttrib> layout;
	layout.push_back(ur::VertexAttrib("position", 3, sizeof(float),    16, 0));
	layout.push_back(ur::VertexAttrib("color",    4, sizeof(uint8_t),  16, 12));
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
	auto col_in_uv = std::make_shared<sw::node::Input>("color", sw::t_flt4);
	auto col_out_uv = std::make_shared<sw::node::Output>("v_color", sw::t_flt4);
	sw::make_connecting({ col_in_uv, 0 }, { col_out_uv, 0 });
	vert_nodes.push_back(col_out_uv);

	// frag
	auto frag_in_col = std::make_shared<sw::node::Input>("v_color", sw::t_flt4);

	// end
	sw::Evaluator vert(vert_nodes, sw::ST_VERT);
	sw::Evaluator frag({ frag_in_col }, sw::ST_FRAG);

	//printf("//////////////////////////////////////////////////////////////////////////\n");
	//printf("%s\n", vert.GenShaderStr().c_str());
	//printf("//////////////////////////////////////////////////////////////////////////\n");
	//printf("%s\n", frag.GenShaderStr().c_str());
	//printf("//////////////////////////////////////////////////////////////////////////\n");

	std::vector<std::string> texture_names;
	pt3::Shader::Params sp(texture_names, layout);
	sp.vs = vert.GenShaderStr().c_str();
	sp.fs = frag.GenShaderStr().c_str();

	sp.uniform_names.model_mat = "u_model";
	sp.uniform_names.view_mat  = "u_view";
	sp.uniform_names.proj_mat  = "u_projection";

	m_shader = std::make_shared<pt3::Shader>(&rc, sp);
}

}