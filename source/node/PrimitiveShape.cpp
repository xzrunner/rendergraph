#include "rendergraph/node/PrimitiveShape.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Device.h>
#include <unirender/Context.h>
#include <unirender/DrawState.h>
#include <unirender/VertexInputAttribute.h>
#include <unirender/IndexBuffer.h>
#include <unirender/VertexBuffer.h>
#include <unirender/VertexArray.h>
#include <unirender/ShaderProgram.h>
#include <painting3/WindowContext.h>
#include <model/ParametricSurface.h>
#include <model/typedef.h>
#include <model/ParametricEquations.h>

namespace
{

struct Vertex
{
    Vertex() {}
    Vertex(size_t ix, size_t iz, size_t size_x, size_t size_z)
    {
        //position = sm::vec3(
        //    ix / static_cast<float>(size_x),
        //    0,
        //    iz / static_cast<float>(size_z)
        //);
        position = sm::vec3(
            ix / static_cast<float>(size_x),
            iz / static_cast<float>(size_z),
            0
        );
        texcoords = sm::vec2(
            ix / static_cast<float>(size_x),
            iz / static_cast<float>(size_z)
        );
    }

    sm::vec3 position;
    sm::vec2 texcoords;
};

const int HEIGHT_FIELD_SIZE = 512;

}

namespace rendergraph
{
namespace node
{

void PrimitiveShape::Draw(const RenderContext& rc) const
{
    if (m_dirty) {
        SetupVertices(rc);
        m_dirty = false;
    }

    if (m_va) 
    {
        ur::DrawState ds = rc.ur_ds;
        ds.vertex_array = m_va;

        pt3::WindowContext wnd_ctx;
        wnd_ctx.SetProjection(rc.cam_proj_mat);
        wnd_ctx.SetView(rc.cam_view_mat);
        wnd_ctx.SetScreen(
            static_cast<int>(rc.screen_size.x), 
            static_cast<int>(rc.screen_size.y)
        );
        if (rc.ur_ds.program->HasStage(ur::ShaderType::TessEvalShader)) {
            rc.ur_ctx->Draw(ur::PrimitiveType::Patches, ds, &wnd_ctx);
        } else {
            rc.ur_ctx->Draw(ur::PrimitiveType::Triangles, ds, &wnd_ctx);
        }
    }
}

void PrimitiveShape::SetType(Type type) 
{
    m_type = type;

    m_dirty = true;
}

void PrimitiveShape::SetVertLayout(VertLayout layout) 
{
    m_vert_layout = layout;

    m_dirty = true;
}

void PrimitiveShape::SetupVertices(const RenderContext& rc) const
{
    ur::VertexLayoutType layout;
    switch (m_vert_layout)
    {
    case VertLayout::Pos:
        layout = ur::VertexLayoutType::Pos;
        break;
    case VertLayout::PosTex:
        layout = ur::VertexLayoutType::PosTex;
        break;
    case VertLayout::PosNorm:
        layout = ur::VertexLayoutType::PosNorm;
        break;
    case VertLayout::PosNormTex:
        layout = ur::VertexLayoutType::PosNormTex;
        break;
    case VertLayout::PosNormTexTB:
        layout = ur::VertexLayoutType::PosNormTexTB;
        break;
    default:
        assert(0);
    }

    m_va.reset();
    ur::PrimitiveType prim_type = ur::PrimitiveType::TriangleStrip;
    switch (m_type)
    {
    case Type::Quad:
        m_prim_type = ur::PrimitiveType::TriangleStrip;
        m_va = rc.ur_dev->GetVertexArray(ur::Device::PrimitiveType::Quad, layout);
        break;
    case Type::Cube:
        m_prim_type = ur::PrimitiveType::Triangles;
        m_va = rc.ur_dev->GetVertexArray(ur::Device::PrimitiveType::Cube, layout);
        break;
    case Type::Sphere:
    {
        auto sphere = std::make_unique<model::Sphere>(1.0f);
        m_prim_type = ur::PrimitiveType::Triangles;
        m_va = BuildVertexArray(*rc.ur_dev, *sphere, m_vert_layout);
    }
        break;
    case Type::Grids:
        m_prim_type = ur::PrimitiveType::Triangles;
        m_va = BuildGridsVA(*rc.ur_dev, m_vert_layout);
        break;
    default:
        assert(0);
    }
}

std::shared_ptr<ur::VertexArray>
PrimitiveShape::BuildVertexArray(const ur::Device& dev, const model::ParametricSurface& surface, VertLayout layout)
{
    int vertex_type = 0;
    int stride = 3;
    switch (layout)
    {
    case VertLayout::Pos:
        vertex_type = 0;
        stride = 3;
        break;
    case VertLayout::PosTex:
        vertex_type = model::VERTEX_FLAG_TEXCOORDS;
        stride = 3 + 2;
        break;
    case VertLayout::PosNorm:
        vertex_type = model::VERTEX_FLAG_NORMALS;
        stride = 3 + 3;
        break;
    case VertLayout::PosNormTex:
        vertex_type = model::VERTEX_FLAG_NORMALS | model::VERTEX_FLAG_TEXCOORDS;
        stride = 3 + 3 + 2;
        break;
    default:
        return nullptr;
    }
    std::vector<unsigned short> indices;
    surface.GenerateTriangleIndices(indices);

	std::vector<float> vertices;
	surface.GenerateVertices(vertex_type, vertices);

    auto va = dev.CreateVertexArray();

    auto usage = ur::BufferUsageHint::StaticDraw;

    auto ibuf_sz = sizeof(unsigned short) * indices.size();
    auto ibuf = dev.CreateIndexBuffer(usage, ibuf_sz);
    ibuf->ReadFromMemory(indices.data(), ibuf_sz, 0);
    va->SetIndexBuffer(ibuf);

    auto vbuf_sz = sizeof(float) * vertices.size();
    auto vbuf = dev.CreateVertexBuffer(ur::BufferUsageHint::StaticDraw, vbuf_sz);
    vbuf->ReadFromMemory(vertices.data(), vbuf_sz, 0);
    va->SetVertexBuffer(vbuf);

    std::vector<std::shared_ptr<ur::VertexInputAttribute>> vbuf_attrs;
    int ptr = 0;
    const int size = stride * 4;
    vbuf_attrs.push_back(std::make_shared<ur::VertexInputAttribute>(
        0, ur::ComponentDataType::Float, 3, ptr, size
    ));
    ptr += 3 * 4;
    if (vertex_type & model::VERTEX_FLAG_NORMALS) 
    {
        vbuf_attrs.push_back(std::make_shared<ur::VertexInputAttribute>(
            vbuf_attrs.size(), ur::ComponentDataType::Float, 3, ptr, size
        ));
        ptr += 3 * 4;
    }
    if (vertex_type & model::VERTEX_FLAG_TEXCOORDS)
    {
        vbuf_attrs.push_back(std::make_shared<ur::VertexInputAttribute>(
            vbuf_attrs.size(), ur::ComponentDataType::Float, 2, ptr, size
        ));
        ptr += 2 * 4;
    }
    va->SetVertexBufferAttrs(vbuf_attrs);

    return va;
}

std::shared_ptr<ur::VertexArray> 
PrimitiveShape::BuildGridsVA(const ur::Device& dev, VertLayout layout)
{
   auto va = dev.CreateVertexArray();

    auto usage = ur::BufferUsageHint::StaticDraw;

    const size_t num_vert = HEIGHT_FIELD_SIZE * HEIGHT_FIELD_SIZE;
    Vertex* vertices = new Vertex[num_vert];
    auto vert_ptr = vertices;
    for (size_t y = 0; y < HEIGHT_FIELD_SIZE; ++y) {
        for (size_t x = 0; x < HEIGHT_FIELD_SIZE; ++x) {
            *vert_ptr++ = Vertex(x, y, HEIGHT_FIELD_SIZE, HEIGHT_FIELD_SIZE);
        }
    }

    auto vbuf = dev.CreateVertexBuffer(usage, 0);
    auto vbuf_sz = sizeof(Vertex) * num_vert;
    vbuf->Reset(vbuf_sz);
    vbuf->ReadFromMemory(vertices, vbuf_sz, 0);
    va->SetVertexBuffer(vbuf);

    const size_t num_idx = (HEIGHT_FIELD_SIZE - 1) * (HEIGHT_FIELD_SIZE - 1) * 6;
    unsigned int* indices = new unsigned int[num_idx];
    auto index_ptr = indices;
    for (size_t y = 0; y < HEIGHT_FIELD_SIZE - 1; ++y) {
        for (size_t x = 0; x < HEIGHT_FIELD_SIZE - 1; ++x) {
            size_t ll = y * HEIGHT_FIELD_SIZE + x;
            size_t rl = ll + 1;
            size_t lh = ll + HEIGHT_FIELD_SIZE;
            size_t rh = lh + 1;
            *index_ptr++ = ll;
            *index_ptr++ = lh;
            *index_ptr++ = rh;
            *index_ptr++ = ll;
            *index_ptr++ = rh;
            *index_ptr++ = rl;
        }
    }

    auto ibuf = dev.CreateIndexBuffer(usage, 0);
    auto ibuf_sz = sizeof(unsigned int) * num_idx;
    ibuf->Reset(ibuf_sz);
    ibuf->ReadFromMemory(indices, ibuf_sz, 0);
    ibuf->SetDataType(ur::IndexBufferDataType::UnsignedInt);
    va->SetIndexBuffer(ibuf);

    va->SetVertexBufferAttrs({
        // pos
        std::make_shared<ur::VertexInputAttribute>(0, ur::ComponentDataType::Float, 3, 0, 20),
        // uv
        std::make_shared<ur::VertexInputAttribute>(1, ur::ComponentDataType::Float, 2, 12, 20)
    });

    return va;
}

}
}