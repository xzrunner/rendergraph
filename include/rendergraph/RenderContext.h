#pragma once

#include <SM_Matrix.h>

#include <boost/noncopyable.hpp>

namespace ur { class RenderContext; }

namespace rendergraph
{

class RenderContext : boost::noncopyable
{
public:
    RenderContext(ur::RenderContext& rc) : rc(rc) {}

    ur::RenderContext& rc;

    sm::mat4 cam_proj_mat;
    sm::mat4 cam_view_mat;
    sm::mat4 model_mat;

    sm::vec3 cam_position;
    sm::vec3 light_position;

}; // RenderContext

}