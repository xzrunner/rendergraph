#pragma once

#include "rendergraph/IRenderer.h"
#include "rendergraph/RendererImpl.h"

#include <SM_Vector.h>
#include <painting0/Material.h>
#include <painting0/RenderContext.h>

#include <boost/noncopyable.hpp>

namespace model { struct MeshGeometry; }

namespace rg
{

struct SkinVertex
{
    sm::vec3 position;
    sm::vec3 normal;
    sm::vec2 texcoord;
    uint32_t blend_indices;
    uint32_t blend_weights;
};

class SkinRenderer : public IRenderer, private RendererImpl<SkinVertex>, private boost::noncopyable
{
public:
    SkinRenderer();

    virtual void Flush() override;

    void Draw(const model::MeshGeometry& mesh, const pt0::Material& material,
        const pt0::RenderContext& ctx) const;

private:
    void InitShader();

}; // SkinRenderer

}