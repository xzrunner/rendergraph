#pragma once

#include "rendergraph/Node.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>

namespace rendergraph
{
namespace node
{

class String : public Node
{
public:
    String()
    {
        m_imports = {
            {{ VariableType::String, "in" }}
        };
        m_exports = {
            {{ VariableType::String, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
                      ShaderVariant& var, uint32_t& flags) const override
    {
        if (var.type != VariableType::String) {
            return;
        }

        if (var.p) {
            delete static_cast<const char*>(var.p);
        }

        auto& conns = m_imports[0].conns;
        if (!conns.empty()) 
        {
            auto node = conns[0].node.lock();
            if (node) 
            {
                auto ret = ShaderVariant(std::string(""));
                std::static_pointer_cast<Node>(node)->Eval(rc, conns[0].idx, ret, flags);
                std::string str = static_cast<const char*>(ret.p);
                str += m_val;
                var.p = _strdup(str.c_str());
                return;
            }
        }

        var.p = _strdup(static_cast<const char*>(m_val.c_str()));
    }

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/String.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // String

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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Int.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Bool.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Vector1.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Vector2.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Vector3.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Vector4.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Matrix2.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Matrix2

class Matrix3 : public Node
{
public:
    Matrix3()
    {
        m_imports = {
            {{ VariableType::Matrix4, "in" }}
        };
        m_exports = {
            {{ VariableType::Matrix3, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Matrix3.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Matrix3

class Matrix4 : public Node
{
public:
    Matrix4()
    {
        m_imports = {
            {{ VariableType::Matrix3, "in" }}
        };
        m_exports = {
            {{ VariableType::Matrix4, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Matrix4.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Matrix4

}
}