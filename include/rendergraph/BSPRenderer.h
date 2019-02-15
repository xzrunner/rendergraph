#pragma once

#include "rendergraph/IRenderer.h"
#include "rendergraph/RendererImpl.h"

#include <SM_Vector.h>

#include <boost/noncopyable.hpp>

namespace rg
{

struct BSPVertex
{
    sm::vec3 position;
    sm::vec2 texcoord;
    sm::vec2 texcoord_light;
};

class BSPRenderer : public IRenderer, private RendererImpl<BSPVertex>, private boost::noncopyable
{
public:
    BSPRenderer();

    virtual void Flush() override {}

    void Draw() const;

private:
    void InitShader();

}; // BSPRenderer

}