#pragma once

#include "rendergraph/Node.h"
#include "rendergraph/Evaluator.h"

#include <SM_Vector.h>
#include <sm_const.h>

namespace rendergraph
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
        if (var.type == VariableType::Matrix4)
        {
            auto fovy   = Evaluator::Calc(rc, m_imports[node::PerspectiveMat::ID_FOVY],   ShaderVariant(m_fovy),   flags);
            auto aspect = Evaluator::Calc(rc, m_imports[node::PerspectiveMat::ID_ASPECT], ShaderVariant(m_aspect), flags);
            auto znear  = Evaluator::Calc(rc, m_imports[node::PerspectiveMat::ID_NEAR],   ShaderVariant(m_znear),  flags);
            auto zfar   = Evaluator::Calc(rc, m_imports[node::PerspectiveMat::ID_FAR],    ShaderVariant(m_zfar),   flags);
            var.mat4 = sm::mat4::Perspective(fovy.vec1, aspect.vec1, znear.vec1, zfar.vec1);
        }
    }

    enum InputID
    {
        ID_FOVY = 0,
        ID_ASPECT,
        ID_NEAR,
        ID_FAR
    };

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/PerspectiveMat.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

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
        if (var.type == VariableType::Matrix4)
        {
            auto left   = Evaluator::Calc(rc, m_imports[node::OrthoMat::ID_LEFT],   ShaderVariant(m_left),   flags);
            auto right  = Evaluator::Calc(rc, m_imports[node::OrthoMat::ID_RIGHT],  ShaderVariant(m_right),  flags);
            auto bottom = Evaluator::Calc(rc, m_imports[node::OrthoMat::ID_BOTTOM], ShaderVariant(m_bottom), flags);
            auto top    = Evaluator::Calc(rc, m_imports[node::OrthoMat::ID_TOP],    ShaderVariant(m_top),    flags);
            auto znear  = Evaluator::Calc(rc, m_imports[node::OrthoMat::ID_ZNEAR],  ShaderVariant(m_znear),  flags);
            auto zfar   = Evaluator::Calc(rc, m_imports[node::OrthoMat::ID_ZFAR],   ShaderVariant(m_zfar),   flags);
            var.mat4 = sm::mat4::Orthographic(left.vec1, right.vec1, bottom.vec1, top.vec1, znear.vec1, zfar.vec1);
        }
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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/OrthoMat.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

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
        if (var.type == VariableType::Matrix4)
        {
            auto eye    = Evaluator::Calc(rc, m_imports[node::LookAtMat::ID_EYE],    ShaderVariant(m_eye),    flags);
            auto center = Evaluator::Calc(rc, m_imports[node::LookAtMat::ID_CENTER], ShaderVariant(m_center), flags);
            auto up     = Evaluator::Calc(rc, m_imports[node::LookAtMat::ID_UP],     ShaderVariant(m_up),     flags);
            var.mat4 = sm::mat4::LookAt(eye.vec3, center.vec3, up.vec3);
        }
    }

    enum InputID
    {
        ID_EYE = 0,
        ID_CENTER,
        ID_UP,
    };

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/LookAtMat.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // LookAtMat

class Translate : public Node
{
public:
    enum InputID
    {
        I_IN = 0,
        I_OFFSET,

        I_MAX_NUM
    };

    enum OutputID
    {
        O_OUT = 0,

        O_MAX_NUM
    };

public:
    Translate()
    {
        m_imports.resize(I_MAX_NUM);
        m_imports[I_IN]     = {{ VariableType::Any,     "in" }};
        m_imports[I_OFFSET] = {{ VariableType::Vector3, "offset" }};

        m_exports.resize(O_MAX_NUM);
        m_exports[O_OUT]    = {{ VariableType::Any,     "out" }};
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        auto input = Evaluator::Calc(rc, m_imports[I_IN], Evaluator::DefaultValue(var.type), flags);
        sm::vec3 offset = m_offset;
        if (!m_imports[I_OFFSET].conns.empty()) {
            auto offset_var = Evaluator::Calc(rc, m_imports[I_OFFSET], Evaluator::DefaultValue(VariableType::Vector3), flags);
            offset = offset_var.vec3;
        }
        switch (var.type)
        {
        case VariableType::Vector3:
            var.vec3 = input.vec3 + offset;
            break;
        case VariableType::Matrix4:
            var.mat4 = sm::mat4::Translated(offset.x, offset.y, offset.z) * input.mat4;
            break;
        default:
            assert(0);
        }
    }

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Translate.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

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
            var.mat4 = sm::mat4::RotatedAxis(m_axis, m_angle) * input.mat4;
            break;
        default:
            assert(0);
        }
    }

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Rotate.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

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
            var.mat4 = sm::mat4::Scaled(m_scale.x, m_scale.y, m_scale.z) * input.mat4;
            break;
        default:
            assert(0);
        }
    }

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Scale.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Scale

}
}