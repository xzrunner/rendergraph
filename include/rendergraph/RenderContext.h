#pragma once

#include <SM_Matrix.h>
#include <dag/Context.h>
#include <unirender2/RenderState.h>

#include <boost/noncopyable.hpp>

#include <memory>

namespace ur2 { class Context; class Device; }

namespace rendergraph
{

class RenderContext : public dag::Context
{
public:
    ur2::RenderState ur_rs;
    std::shared_ptr<ur2::Context> ur_ctx = nullptr;
    const ur2::Device* ur_dev = nullptr;

    sm::mat4 cam_proj_mat;
    sm::mat4 cam_view_mat;
    sm::mat4 model_mat;

    sm::vec3 cam_position;
    sm::vec3 light_position;

}; // RenderContext

}