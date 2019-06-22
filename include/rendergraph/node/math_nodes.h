#pragma once

#include "rendergraph/Node.h"
#include "rendergraph/Evaluator.h"

#include <SM_Vector.h>
#include <sm_const.h>

namespace rg
{
namespace node
{

class Add : public Node
{
public:
    Add()
    {
        m_imports = {
            {{ VariableType::Any, "a" }},
            {{ VariableType::Any, "b" }}
        };
        m_exports = {
            {{ VariableType::Any, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        auto a = Evaluator::Calc(rc, m_imports[0], Evaluator::DefaultValue(var.type), flags);
        auto b = Evaluator::Calc(rc, m_imports[1], Evaluator::DefaultValue(var.type), flags);
        switch (var.type)
        {
        case VariableType::Vector1:
            var.vec1 = a.vec1 + b.vec1;
            break;
        case VariableType::Vector2:
            var.vec2 = a.vec2 + b.vec2;
            break;
        case VariableType::Vector3:
            var.vec3 = a.vec3 + b.vec3;
            break;
        }
    }

    RTTR_ENABLE(Node)

}; // Add

class Mul : public Node
{
public:
    Mul()
    {
        m_imports = {
            {{ VariableType::Any, "a" }},
            {{ VariableType::Any, "b" }}
        };
        m_exports = {
            {{ VariableType::Any, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        auto a = Evaluator::Calc(rc, m_imports[0], Evaluator::DefaultValue(var.type), flags);
        auto b = Evaluator::Calc(rc, m_imports[1], Evaluator::DefaultValue(var.type), flags);
        switch (var.type)
        {
        case VariableType::Vector1:
            var.vec1 = a.vec1 * b.vec1;
            break;
        case VariableType::Matrix4:
            var.mat4 = a.mat4 * b.mat4;
            break;
        }
    }

    RTTR_ENABLE(Node)

}; // Mul

class PerspectiveMat : public Node
{
public:
    PerspectiveMat()
    {
        m_imports = {
            {{ VariableType::Vector1, "fovy" }},
            {{ VariableType::Vector1, "aspect" }},
            {{ VariableType::Vector1, "near" }},
            {{ VariableType::Vector1, "far" }}
        };
        m_exports = {
            {{ VariableType::Matrix4, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        auto fovy   = Evaluator::Calc(rc, m_imports[node::PerspectiveMat::ID_FOVY],   ShaderVariant(m_fovy),   flags);
        auto aspect = Evaluator::Calc(rc, m_imports[node::PerspectiveMat::ID_ASPECT], ShaderVariant(m_aspect), flags);
        auto znear  = Evaluator::Calc(rc, m_imports[node::PerspectiveMat::ID_NEAR],   ShaderVariant(m_znear),  flags);
        auto zfar   = Evaluator::Calc(rc, m_imports[node::PerspectiveMat::ID_FAR],    ShaderVariant(m_zfar),   flags);
        var.mat4 = sm::mat4::Perspective(fovy.vec1, aspect.vec1, znear.vec1, zfar.vec1);
    }

    void SetProps(float fovy, float aspect, float znear, float zfar) {
        m_fovy   = fovy;
        m_aspect = aspect;
        m_znear  = znear;
        m_zfar   = zfar;
    }

    enum InputID
    {
        ID_FOVY = 0,
        ID_ASPECT,
        ID_NEAR,
        ID_FAR
    };

private:
    float m_fovy   = 45.0f * SM_DEG_TO_RAD;
    float m_aspect = 1.0f;
    float m_znear  = 0.1f;
    float m_zfar   = 100.0f;

    RTTR_ENABLE(Node)

}; // PerspectiveMat

class OrthoMat : public Node
{
public:
    OrthoMat()
    {
        m_imports = {
            {{ VariableType::Vector1, "left" }},
            {{ VariableType::Vector1, "right" }},
            {{ VariableType::Vector1, "bottom" }},
            {{ VariableType::Vector1, "top" }},
            {{ VariableType::Vector1, "znear" }},
            {{ VariableType::Vector1, "zfar" }}
        };
        m_exports = {
            {{ VariableType::Matrix4, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        auto left   = Evaluator::Calc(rc, m_imports[node::OrthoMat::ID_LEFT],   ShaderVariant(m_left),   flags);
        auto right  = Evaluator::Calc(rc, m_imports[node::OrthoMat::ID_RIGHT],  ShaderVariant(m_right),  flags);
        auto bottom = Evaluator::Calc(rc, m_imports[node::OrthoMat::ID_BOTTOM], ShaderVariant(m_bottom), flags);
        auto top    = Evaluator::Calc(rc, m_imports[node::OrthoMat::ID_TOP],    ShaderVariant(m_top),    flags);
        auto znear  = Evaluator::Calc(rc, m_imports[node::OrthoMat::ID_ZNEAR],  ShaderVariant(m_znear),  flags);
        auto zfar   = Evaluator::Calc(rc, m_imports[node::OrthoMat::ID_ZFAR],   ShaderVariant(m_zfar),   flags);
        var.mat4 = sm::mat4::Orthographic(left.vec1, right.vec1, bottom.vec1, top.vec1, znear.vec1, zfar.vec1);
    }

    void SetProps(float left, float right, float bottom, float top, float znear, float zfar) {
        m_left   = left;
        m_right  = right;
        m_bottom = bottom;
        m_top    = top;
        m_znear  = znear;
        m_zfar   = zfar;
    }

    enum InputID
    {
        ID_LEFT = 0,
        ID_RIGHT,
        ID_BOTTOM,
        ID_TOP,
        ID_ZNEAR,
        ID_ZFAR
    };

private:
    float m_left   = -10.0f;
    float m_right  = 10.0f;
    float m_bottom = -10.0f;
    float m_top    = 10.0f;
    float m_znear  = 1.0f;
    float m_zfar   = 7.5f;

    RTTR_ENABLE(Node)

}; // OrthoMat

class LookAtMat : public Node
{
public:
    LookAtMat()
    {
        m_imports = {
            {{ VariableType::Vector3, "eye" }},
            {{ VariableType::Vector3, "center" }},
            {{ VariableType::Vector3, "up" }}
        };
        m_exports = {
            {{ VariableType::Matrix4, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        auto eye    = Evaluator::Calc(rc, m_imports[node::LookAtMat::ID_EYE],    ShaderVariant(m_eye),    flags);
        auto center = Evaluator::Calc(rc, m_imports[node::LookAtMat::ID_CENTER], ShaderVariant(m_center), flags);
        auto up     = Evaluator::Calc(rc, m_imports[node::LookAtMat::ID_UP],     ShaderVariant(m_up),     flags);
        var.mat4 = sm::mat4::LookAt(eye.vec3, center.vec3, up.vec3);
    }

    void SetProps(const sm::vec3& eye, const sm::vec3& center, const sm::vec3& up) {
        m_eye    = eye;
        m_center = center;
        m_up     = up;
    }

    enum InputID
    {
        ID_EYE = 0,
        ID_CENTER,
        ID_UP,
    };

private:
    sm::vec3 m_eye;
    sm::vec3 m_center;
    sm::vec3 m_up = sm::vec3(0, 1, 0);

    RTTR_ENABLE(Node)

}; // LookAtMat

class Translate : public Node
{
public:
    Translate()
    {
        m_imports = {
            {{ VariableType::Any, "in" }},
        };
        m_exports = {
            {{ VariableType::Any, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        auto input = Evaluator::Calc(rc, m_imports[0], Evaluator::DefaultValue(var.type), flags);
        switch (var.type)
        {
        case VariableType::Vector3:
            var.vec3 = input.vec3 + m_offset;
            break;
        case VariableType::Matrix4:
            var.mat4 = input.mat4 * sm::mat4::Translated(m_offset.x, m_offset.y, m_offset.z);
            break;
        default:
            assert(0);
        }
    }

    void SetOffset(const sm::vec3& offset) { m_offset = offset; }

private:
    sm::vec3 m_offset;

    RTTR_ENABLE(Node)

}; // Translate

class Rotate : public Node
{
public:
    Rotate()
    {
        m_imports = {
            {{ VariableType::Any, "in" }},
        };
        m_exports = {
            {{ VariableType::Any, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        auto input = Evaluator::Calc(rc, m_imports[0], Evaluator::DefaultValue(var.type), flags);
        switch (input.type)
        {
        case VariableType::Matrix4:
            var.mat4 = input.mat4 * sm::mat4::RotatedAxis(m_axis, m_angle);
            break;
        default:
            assert(0);
        }
    }

    void SetAngle(float angle) { m_angle = angle; }
    void SetAxis(const sm::vec3& axis) { m_axis = axis; }

private:
    float    m_angle = 0;
    sm::vec3 m_axis = sm::vec3(1, 0, 0);

    RTTR_ENABLE(Node)

}; // Rotate

class Scale : public Node
{
public:
    Scale()
    {
        m_imports = {
            {{ VariableType::Any, "in" }},
        };
        m_exports = {
            {{ VariableType::Any, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        auto input = Evaluator::Calc(rc, m_imports[0], Evaluator::DefaultValue(var.type), flags);
        switch (var.type)
        {
        case VariableType::Vector3:
            for (int i = 0; i < 3; ++i) {
                var.vec3.xyz[i] = input.vec3.xyz[i] * m_scale.xyz[i];
            }
            break;
        case VariableType::Matrix4:
            var.mat4 = input.mat4 * sm::mat4::Scaled(m_scale.x, m_scale.y, m_scale.z);
            break;
        default:
            assert(0);
        }
    }

    void SetScale(const sm::vec3& scale) { m_scale = scale; }

private:
    sm::vec3 m_scale;

    RTTR_ENABLE(Node)

}; // Scale

}
}