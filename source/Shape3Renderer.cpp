#include "rendergraph/Shape3Renderer.h"
#include "rendergraph/UniformNames.h"

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
#include <shaderweaver/node/VertexShader.h>
#include <shaderweaver/node/FragmentShader.h>

namespace rg
{

Shape3Renderer::Shape3Renderer()
{
    InitShader();
}

void Shape3Renderer::Flush()
{
    FlushBuffer(m_draw_mode, m_shader);
}

void Shape3Renderer::DrawLines(size_t num, const float* positions, uint32_t color)
{
    if (m_draw_mode != ur::DRAW_LINES) {
        Flush();
        m_draw_mode = ur::DRAW_LINES;
    }

    if (m_buf.vertices.size() + num >= RenderBuffer<Shape3Vertex>::MAX_VERTEX_NUM) {
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

        *m_buf.index_ptr++ = m_buf.curr_index + static_cast<unsigned short>(i);
    }
    m_buf.curr_index += static_cast<unsigned short>(num);
}

void Shape3Renderer::InitShader()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	// layout
	std::vector<ur::VertexAttrib> layout;
	layout.push_back(ur::VertexAttrib(VERT_POSITION_NAME, 3, sizeof(float),    16, 0));
	layout.push_back(ur::VertexAttrib(VERT_COLOR_NAME,    4, sizeof(uint8_t),  16, 12));
	auto layout_id = rc.CreateVertexLayout(layout);
	rc.BindVertexLayout(layout_id);

	// vert
	std::vector<sw::NodePtr> vert_nodes;

	auto projection = std::make_shared<sw::node::Uniform>(PROJ_MAT_NAME, sw::t_mat4);
	auto view       = std::make_shared<sw::node::Uniform>(VIEW_MAT_NAME,       sw::t_mat4);
	auto model      = std::make_shared<sw::node::Uniform>(MODEL_MAT_NAME,      sw::t_mat4);

	auto position   = std::make_shared<sw::node::Input>  (VERT_POSITION_NAME,     sw::t_pos3);

	auto pos_trans = std::make_shared<sw::node::PositionTrans>(4);
	sw::make_connecting({ projection, 0 }, { pos_trans, sw::node::PositionTrans::ID_PROJ });
	sw::make_connecting({ view,       0 }, { pos_trans, sw::node::PositionTrans::ID_VIEW });
	sw::make_connecting({ model,      0 }, { pos_trans, sw::node::PositionTrans::ID_MODEL });
	sw::make_connecting({ position,   0 }, { pos_trans, sw::node::PositionTrans::ID_POS });
    auto vert_end = std::make_shared<sw::node::VertexShader>();
    sw::make_connecting({ pos_trans, 0 }, { vert_end, 0 });
	vert_nodes.push_back(vert_end);

	// varying
	auto col_in_uv = std::make_shared<sw::node::Input>(VERT_COLOR_NAME, sw::t_flt4);
	auto col_out_uv = std::make_shared<sw::node::Output>(FRAG_COLOR_NAME, sw::t_flt4);
	sw::make_connecting({ col_in_uv, 0 }, { col_out_uv, 0 });
	vert_nodes.push_back(col_out_uv);

	// frag
	auto frag_in_col = std::make_shared<sw::node::Input>(FRAG_COLOR_NAME, sw::t_flt4);
    auto frag_end = std::make_shared<sw::node::FragmentShader>();
    sw::make_connecting({ frag_in_col, 0 }, { frag_end, 0 });

	// end
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

	m_shader = std::make_shared<pt3::Shader>(&rc, sp);
}

}