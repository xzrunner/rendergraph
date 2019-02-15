#pragma once

#include "rendergraph/IRenderer.h"
#include "rendergraph/RendererImpl.h"

#include <SM_Vector.h>

#include <boost/noncopyable.hpp>

namespace rg
{

struct MorphVertex
{
    sm::vec3 pos1;
    sm::vec3 normal1;
    sm::vec3 pos2;
    sm::vec3 normal2;
    sm::vec2 texcoord;
};

class MorphRenderer : public IRenderer, private RendererImpl<MorphVertex>, private boost::noncopyable
{
public:
    MorphRenderer();

    virtual void Flush() override {}

    void Draw() const;

private:
    void InitShader();

}; // MorphRenderer

}