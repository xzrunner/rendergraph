#include "rendergraph/Evaluator.h"
#include "rendergraph/RenderContext.h"
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
    // value
    if (node_type == rttr::type::get<node::Vector1>())
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

        float distance = (center.vec3 - eye.vec3).Length();
        auto n = (center.vec3 - eye.vec3).Normalized();
        auto v = up.vec3.Normalized();
        auto u = v.Cross(n).Normalized();
        v = n.Cross(u).Normalized();

        float* m = ret.mat4.x;
        m[0] = u.x; m[4] = u.y; m[8] = u.z;  m[12] = 0;
        m[1] = v.x; m[5] = v.y; m[9] = v.z;  m[13] = 0;
        m[2] = n.x; m[6] = n.y; m[10] = n.z; m[14] = 0;
        m[3] = 0;   m[7] = 0;   m[11] = 0;   m[15] = 1.0;
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
    default:
        assert(0);
    }
    return ret;
}

}