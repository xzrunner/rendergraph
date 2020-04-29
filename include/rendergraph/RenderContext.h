#pragma once

#include <SM_Matrix.h>
#include <dag/Context.h>
#include <unirender/DrawState.h>

#include <boost/noncopyable.hpp>

#include <memory>

namespace ur { class Context; class Device; }

namespace rendergraph
{

class RenderContext : public dag::Context
{
public:
    ur::DrawState     ur_ds;
    ur::Context*      ur_ctx = nullptr;
    const ur::Device* ur_dev = nullptr;

    sm::mat4 cam_proj_mat;
    sm::mat4 cam_view_mat;
    sm::mat4 model_mat;

    sm::vec3 cam_position;
    sm::vec3 light_position;

}; // RenderContext

}