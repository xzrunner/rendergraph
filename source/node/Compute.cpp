#include "rendergraph/node/Compute.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/Evaluator.h"
#include "rendergraph/ValueImpl.h"

#include <unirender/Context.h>
#include <unirender/Texture.h>

namespace rendergraph
{
namespace node
{

void Compute::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    auto& conns = m_imports[ID_SHADER].conns;
    if (conns.empty()) {
        return;
    }

    std::shared_ptr<Shader> shader = nullptr;
    if (!m_imports[ID_SHADER].conns.empty())
    {
        auto node = m_imports[ID_SHADER].conns[0].node.lock();
        if (node->get_type() == rttr::type::get<Shader>()) {
            shader = std::static_pointer_cast<Shader>(node);
        }
    }

    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    std::shared_ptr<ur::ShaderProgram> prog = nullptr;
    if (shader) {
        prog = shader->GetShader(*rc);
    }

    if (prog) {
        rc->ur_ds.program = prog;
    }

    ur::TexturePtr out_tex = nullptr;
    for (auto& input : shader->GetImports())
    {
        ShaderVariant var;
        uint32_t flags = 0;
        auto val = Evaluator::Calc(*rc, input, var, flags);
        if (val.type == VariableType::Texture && val.p) 
        {
            auto img = shader->QueryImage(input.var.type.name);
            if (img)
            {
                auto tex = reinterpret_cast<const TextureVal*>(val.p);
                tex->texture->BindToImage(img->unit, img->access);
                if (img->access == ur::AccessType::WriteOnly) {
                    out_tex = tex->texture;
                }
            }
        }
    }

    if (out_tex)
    {
        const int w = out_tex->GetWidth();
        const int h = out_tex->GetHeight();

        auto v_sz_x = shader->QueryProp("local_size_x");
        auto v_sz_y = shader->QueryProp("local_size_y");
        auto v_sz_z = shader->QueryProp("local_size_z");
        assert(v_sz_x.type == VariableType::Int 
            && v_sz_y.type == VariableType::Int
            && v_sz_z.type == VariableType::Int);

        int group_x = v_sz_x.i == 1 ? 1 : (w / v_sz_x.i + 1);
        int group_y = v_sz_y.i == 1 ? 1 : (h / v_sz_y.i + 1);
        assert(v_sz_z.i == 1);
        int group_z = 1;

        rc->ur_ctx->Compute(rc->ur_ds, group_x, group_y, group_z);
    }
}

}
}