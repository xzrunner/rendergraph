#include "rendergraph/Evaluator.h"
#include "rendergraph/node/Value.h"

namespace rg
{

ShaderVariant Evaluator::Calc(const Node::Port& in_port, VariableType type)
{
    auto& conns = in_port.conns;
    if (conns.empty()) {
        return DefaultValue(type);
    }

    auto node = conns[0].node.lock();
    if (!node) {
        return DefaultValue(type);
    }

    ShaderVariant ret = DefaultValue(type);
    auto node_type = node->get_type();
    if (node_type == rttr::type::get<node::Vector1>())
    {
        if (type == VariableType::Vector1) {
            ret.vec1 = std::static_pointer_cast<node::Vector1>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Vector2>())
    {
        if (type == VariableType::Vector2) {
            ret.vec2 = std::static_pointer_cast<node::Vector2>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Vector3>())
    {
        if (type == VariableType::Vector3) {
            ret.vec3 = std::static_pointer_cast<node::Vector3>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Vector4>())
    {
        if (type == VariableType::Vector4) {
            ret.vec4 = std::static_pointer_cast<node::Vector4>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Matrix2>())
    {
        if (type == VariableType::Matrix2) {
            ret.mat2 = std::static_pointer_cast<node::Matrix2>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Matrix3>())
    {
        if (type == VariableType::Matrix3) {
            ret.mat3 = std::static_pointer_cast<node::Matrix3>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Matrix4>())
    {
        if (type == VariableType::Matrix4) {
            ret.mat4 = std::static_pointer_cast<node::Matrix4>(node)->GetValue();
        }
    }
    else if (node_type == rttr::type::get<node::Add>())
    {
        auto& inputs = node->GetImports();
        auto a = Calc(inputs[0], type);
        auto b = Calc(inputs[1], type);
        switch (type)
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
        auto a = Calc(inputs[0], type);
        auto b = Calc(inputs[1], type);
        switch (type)
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
        auto fovy   = Calc(inputs[node::PerspectiveMat::ID_FOVY],   VariableType::Vector1);
        auto aspect = Calc(inputs[node::PerspectiveMat::ID_ASPECT], VariableType::Vector1);
        auto near   = Calc(inputs[node::PerspectiveMat::ID_NEAR],   VariableType::Vector1);
        auto far    = Calc(inputs[node::PerspectiveMat::ID_FAR],    VariableType::Vector1);
        ret.mat4 = sm::mat4::Perspective(fovy.vec1, aspect.vec1, near.vec1, far.vec1);
    }
    else if (node_type == rttr::type::get<node::OrthoMat>())
    {
        auto& inputs = node->GetImports();
        auto left   = Calc(inputs[node::OrthoMat::ID_LEFT],   VariableType::Vector1);
        auto right  = Calc(inputs[node::OrthoMat::ID_RIGHT],  VariableType::Vector1);
        auto bottom = Calc(inputs[node::OrthoMat::ID_BOTTOM], VariableType::Vector1);
        auto top    = Calc(inputs[node::OrthoMat::ID_TOP],    VariableType::Vector1);
        auto znear  = Calc(inputs[node::OrthoMat::ID_ZNEAR],  VariableType::Vector1);
        auto zfar   = Calc(inputs[node::OrthoMat::ID_ZFAR],   VariableType::Vector1);
        ret.mat4 = sm::mat4::Orthographic(left.vec1, right.vec1, bottom.vec1, top.vec1, znear.vec1, zfar.vec1);
    }
    else if (node_type == rttr::type::get<node::LookAtMat>())
    {
        auto& inputs = node->GetImports();
        auto eye    = Calc(inputs[node::LookAtMat::ID_EYE],    VariableType::Vector3);
        auto center = Calc(inputs[node::LookAtMat::ID_CENTER], VariableType::Vector3);
        auto up     = Calc(inputs[node::LookAtMat::ID_UP],     VariableType::Vector3);

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