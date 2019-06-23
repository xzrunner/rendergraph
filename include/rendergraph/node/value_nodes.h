#pragma once

#include "rendergraph/Node.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>

namespace rg
{
namespace node
{

class Int : public Node
{
public:
    Int()
    {
        m_exports = {
            {{ VariableType::Int, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        if (var.type == VariableType::Int) {
            var.i = m_val;
        }
    }

    void SetValue(int val) { m_val = val; }

private:
    int m_val = false;

    RTTR_ENABLE(Node)

}; // Int

class Bool : public Node
{
public:
    Bool()
    {
        m_exports = {
            {{ VariableType::Bool, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        if (var.type == VariableType::Bool) {
            var.b = m_val;
        }
    }

    void SetValue(bool val) { m_val = val; }

private:
    bool m_val = false;

    RTTR_ENABLE(Node)

}; // Bool

class Vector1 : public Node
{
public:
    Vector1()
    {
        m_exports = {
            {{ VariableType::Vector1, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        if (var.type == VariableType::Vector1) {
            var.vec1 = m_val;
        }
    }

    void SetValue(float val) { m_val = val; }

private:
    float m_val = 0;

    RTTR_ENABLE(Node)

}; // Vector1

class Vector2 : public Node
{
public:
    Vector2()
    {
        m_exports = {
            {{ VariableType::Vector2, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        if (var.type == VariableType::Vector2) {
            var.vec2 = m_val;
        }
    }

    void SetValue(const sm::vec2& val) { m_val = val; }

private:
    sm::vec2 m_val;

    RTTR_ENABLE(Node)

}; // Vector2

class Vector3 : public Node
{
public:
    Vector3()
    {
        m_exports = {
            {{ VariableType::Vector3, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        if (var.type == VariableType::Vector3) {
            var.vec3 = m_val;
        }
    }

    void SetValue(const sm::vec3& val) { m_val = val; }

private:
    sm::vec3 m_val;

    RTTR_ENABLE(Node)

}; // Vector3

class Vector4 : public Node
{
public:
    Vector4()
    {
        m_exports = {
            {{ VariableType::Vector4, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        if (var.type == VariableType::Vector4) {
            var.vec4 = m_val;
        }
    }

    void SetValue(const sm::vec4& val) { m_val = val; }

private:
    sm::vec4 m_val;

    RTTR_ENABLE(Node)

}; // Vector4

class Matrix2 : public Node
{
public:
    Matrix2()
    {
        m_exports = {
            {{ VariableType::Matrix2, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        if (var.type == VariableType::Matrix2) {
            var.mat2 = m_val;
        }
    }

    void SetValue(const sm::mat2& val) { m_val = val; }

private:
    sm::mat2 m_val;

    RTTR_ENABLE(Node)

}; // Matrix2

class Matrix3 : public Node
{
public:
    Matrix3()
    {
        m_exports = {
            {{ VariableType::Matrix3, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        if (var.type == VariableType::Matrix3) {
            var.mat3 = m_val;
        }
    }

    void SetValue(const sm::mat3& val) { m_val = val; }

private:
    sm::mat3 m_val;

    RTTR_ENABLE(Node)

}; // Matrix3

class Matrix4 : public Node
{
public:
    Matrix4()
    {
        m_exports = {
            {{ VariableType::Matrix4, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        if (var.type == VariableType::Matrix4) {
            var.mat4 = m_val;
        }
    }

    void SetValue(const sm::mat4& val) { m_val = val; }

private:
    sm::mat4 m_val;

    RTTR_ENABLE(Node)

}; // Matrix4

}
}