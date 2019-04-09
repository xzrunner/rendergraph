#pragma once

#include "rendergraph/Node.h"

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

    virtual void Execute(const RenderContext& rc) override {}

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

    virtual void Execute(const RenderContext& rc) override {}

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

    virtual void Execute(const RenderContext& rc) override {}

    enum InputID
    {
        ID_FOVY = 0,
        ID_ASPECT,
        ID_NEAR,
        ID_FAR
    };

    float fovy   = 45.0f * SM_DEG_TO_RAD;
    float aspect = 1.0f;
    float znear  = 0.1f;
    float zfar   = 100.0f;

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

    virtual void Execute(const RenderContext& rc) override {}

    enum InputID
    {
        ID_LEFT = 0,
        ID_RIGHT,
        ID_BOTTOM,
        ID_TOP,
        ID_ZNEAR,
        ID_ZFAR
    };

    float left   = -10.0f;
    float right  = 10.0f;
    float bottom = -10.0f;
    float top    = 10.0f;
    float znear  = 1.0f;
    float zfar   = 7.5f;

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

    virtual void Execute(const RenderContext& rc) override {}

    enum InputID
    {
        ID_EYE = 0,
        ID_CENTER,
        ID_UP,
    };

    sm::vec3 eye;
    sm::vec3 center;
    sm::vec3 up = sm::vec3(0, 1, 0);

    RTTR_ENABLE(Node)

}; // LookAtMat

}
}