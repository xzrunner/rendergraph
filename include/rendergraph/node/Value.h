#pragma once

#include "rendergraph/Node.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>

namespace rg
{
namespace node
{

class Vector1 : public Node
{
public:
    Vector1()
    {
        m_exports = {
            {{ VariableType::Vector1, "out" }}
        };
    }

    virtual void Execute(ur::RenderContext& rc) override {}

    auto& GetValue() const { return m_val; }

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

    virtual void Execute(ur::RenderContext& rc) override {}

    auto& GetValue() const { return m_val; }

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

    virtual void Execute(ur::RenderContext& rc) override {}

    auto& GetValue() const { return m_val; }

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

    virtual void Execute(ur::RenderContext& rc) override {}

    auto& GetValue() const { return m_val; }

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

    virtual void Execute(ur::RenderContext& rc) override {}

    auto& GetValue() const { return m_val; }

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

    virtual void Execute(ur::RenderContext& rc) override {}

    auto& GetValue() const { return m_val; }

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

    virtual void Execute(ur::RenderContext& rc) override {}

    auto& GetValue() const { return m_val; }

private:
    sm::mat4 m_val;

    RTTR_ENABLE(Node)

}; // Matrix4

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

    virtual void Execute(ur::RenderContext& rc) override {}

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

    virtual void Execute(ur::RenderContext& rc) override {}

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

    virtual void Execute(ur::RenderContext& rc) override {}

    enum InputID
    {
        ID_FOVY = 0,
        ID_ASPECT,
        ID_NEAR,
        ID_FAR
    };

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

    virtual void Execute(ur::RenderContext& rc) override {}

    enum InputID
    {
        ID_LEFT = 0,
        ID_RIGHT,
        ID_BOTTOM,
        ID_TOP,
        ID_ZNEAR,
        ID_ZFAR
    };

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

    virtual void Execute(ur::RenderContext& rc) override {}

    enum InputID
    {
        ID_EYE = 0,
        ID_CENTER,
        ID_UP,
    };

    RTTR_ENABLE(Node)

}; // LookAtMat

}
}