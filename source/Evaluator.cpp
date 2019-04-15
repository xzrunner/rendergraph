#include "rendergraph/Evaluator.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/value_nodes.h"
#include "rendergraph/node/math_nodes.h"
#include "rendergraph/node/input_nodes.h"

namespace rg
{

ShaderVariant Evaluator::Calc(const RenderContext& rc,
                              const Node::Port& in_port,
                              VariableType type)
{
    return Calc(rc, in_port, DefaultValue(type));
}

ShaderVariant Evaluator::Calc(const RenderContext& rc,
                              const Node::Port& in_port,
                              ShaderVariant expect)
{
    auto& conns = in_port.conns;
    if (conns.empty()) {
        return expect;
    }

    auto node = conns[0].node.lock();
    if (!node) {
        return expect;
    }

    ShaderVariant ret = expect;
    auto node_type = node->get_type();
    // resource
    if (node_type == rttr::type::get<node::Texture>())
    {
        auto tex = std::static_pointer_cast<node::Texture>(node);
        // todo: other texture type
        ret.type = VariableType::Sampler2D;
        ret.id = tex->GetTexID();
    }
    // value
    else if (node_type == rttr::type::get<node::Vector1>())
    {
        if (expect.type == VariableType::Vector1) {
            ret.vec1 = std::static_pointer_cast<node::Vector1>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Vector2>())
    {
        if (expect.type == VariableType::Vector2) {
            ret.vec2 = std::static_pointer_cast<node::Vector2>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Vector3>())
    {
        if (expect.type == VariableType::Vector3) {
            ret.vec3 = std::static_pointer_cast<node::Vector3>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Vector4>())
    {
        if (expect.type == VariableType::Vector4) {
            ret.vec4 = std::static_pointer_cast<node::Vector4>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Matrix2>())
    {
        if (expect.type == VariableType::Matrix2) {
            ret.mat2 = std::static_pointer_cast<node::Matrix2>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Matrix3>())
    {
        if (expect.type == VariableType::Matrix3) {
            ret.mat3 = std::static_pointer_cast<node::Matrix3>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Matrix4>())
    {
        if (expect.type == VariableType::Matrix4) {
            ret.mat4 = std::static_pointer_cast<node::Matrix4>(node)->GetValue();
        }
    }
    // math
    else if (node_type == rttr::type::get<node::Add>())
    {
        auto& inputs = node->GetImports();
        auto a = Calc(rc, inputs[0], DefaultValue(expect.type));
        auto b = Calc(rc, inputs[1], DefaultValue(expect.type));
        switch (expect.type)
        {
        case VariableType::Vector1:
            ret.vec1 = a.vec1 + b.vec1;
            break;
        case VariableType::Vector2:
            ret.vec2 = a.vec2 + b.vec2;
            break;
        case VariableType::Vector3:
            ret.vec3 = a.vec3 + b.vec3;
            break;
        }
    }
    else if (node_type == rttr::type::get<node::Mul>())
    {
        auto& inputs = node->GetImports();
        auto a = Calc(rc, inputs[0], DefaultValue(expect.type));
        auto b = Calc(rc, inputs[1], DefaultValue(expect.type));
        switch (expect.type)
        {
        case VariableType::Vector1:
            ret.vec1 = a.vec1 * b.vec1;
            break;
        case VariableType::Matrix4:
            ret.mat4 = a.mat4 * b.mat4;
            break;
        }
    }
    else if (node_type == rttr::type::get<node::PerspectiveMat>())
    {
        auto& inputs = node->GetImports();
        auto pm = std::static_pointer_cast<node::PerspectiveMat>(node);
        auto fovy   = Calc(rc, inputs[node::PerspectiveMat::ID_FOVY],   ShaderVariant(pm->fovy));
        auto aspect = Calc(rc, inputs[node::PerspectiveMat::ID_ASPECT], ShaderVariant(pm->aspect));
        auto znear  = Calc(rc, inputs[node::PerspectiveMat::ID_NEAR],   ShaderVariant(pm->znear));
        auto zfar   = Calc(rc, inputs[node::PerspectiveMat::ID_FAR],    ShaderVariant(pm->zfar));
        ret.mat4 = sm::mat4::Perspective(fovy.vec1, aspect.vec1, znear.vec1, zfar.vec1);
    }
    else if (node_type == rttr::type::get<node::OrthoMat>())
    {
        auto& inputs = node->GetImports();
        auto om = std::static_pointer_cast<node::OrthoMat>(node);
        auto left   = Calc(rc, inputs[node::OrthoMat::ID_LEFT],   ShaderVariant(om->left));
        auto right  = Calc(rc, inputs[node::OrthoMat::ID_RIGHT],  ShaderVariant(om->right));
        auto bottom = Calc(rc, inputs[node::OrthoMat::ID_BOTTOM], ShaderVariant(om->bottom));
        auto top    = Calc(rc, inputs[node::OrthoMat::ID_TOP],    ShaderVariant(om->top));
        auto znear  = Calc(rc, inputs[node::OrthoMat::ID_ZNEAR],  ShaderVariant(om->znear));
        auto zfar   = Calc(rc, inputs[node::OrthoMat::ID_ZFAR],   ShaderVariant(om->zfar));
        ret.mat4 = sm::mat4::Orthographic(left.vec1, right.vec1, bottom.vec1, top.vec1, znear.vec1, zfar.vec1);
    }
    else if (node_type == rttr::type::get<node::LookAtMat>())
    {
        auto& inputs = node->GetImports();
        auto lm = std::static_pointer_cast<node::LookAtMat>(node);
        auto eye    = Calc(rc, inputs[node::LookAtMat::ID_EYE],    ShaderVariant(lm->eye));
        auto center = Calc(rc, inputs[node::LookAtMat::ID_CENTER], ShaderVariant(lm->center));
        auto up     = Calc(rc, inputs[node::LookAtMat::ID_UP],     ShaderVariant(lm->up));
        ret.mat4 = sm::mat4::LookAt(eye.vec3, center.vec3, up.vec3);
    }
    // input
    else if (node_type == rttr::type::get<node::CamProjMat>())
    {
        ret.mat4 = rc.cam_proj_mat;
    }
    else if (node_type == rttr::type::get<node::CamViewMat>())
    {
        ret.mat4 = rc.cam_view_mat;
    }
    else if (node_type == rttr::type::get<node::CameraPosition>())
    {
        ret.vec3 = rc.cam_position;
    }
    else if (node_type == rttr::type::get<node::LightPosition>())
    {
        ret.vec3 = rc.light_position;
    }

    return ret;
}

ShaderVariant Evaluator::DefaultValue(VariableType type)
{
    ShaderVariant ret;
    switch (type)
    {
    case VariableType::Vector1:
        ret = ShaderVariant(0.0f);
        break;
    case VariableType::Vector2:
        ret = ShaderVariant(sm::vec2());
        break;
    case VariableType::Vector3:
        ret = ShaderVariant(sm::vec3());
        break;
    case VariableType::Vector4:
        ret = ShaderVariant(sm::vec4());
        break;
    case VariableType::Matrix2:
        ret = ShaderVariant(sm::mat2());
        break;
    case VariableType::Matrix3:
        ret = ShaderVariant(sm::mat3());
        break;
    case VariableType::Matrix4:
        ret = ShaderVariant(sm::mat4());
        break;
    case VariableType::Sampler2D:
        break;
    default:
        assert(0);
    }
    return ret;
}

}