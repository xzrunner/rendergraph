#pragma once

#include "rendergraph/VariableType.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>

namespace rg
{

class ShaderVariant
{
public:
    ShaderVariant() : type(VariableType::Any) {}
    ShaderVariant(const ShaderVariant& var) {
        memcpy(this, &var, sizeof(var));
    }
    ShaderVariant& operator = (const ShaderVariant& var) {
        memcpy(this, &var, sizeof(var));
        return *this;
    }

    explicit ShaderVariant(float v) : type(VariableType::Vector1), vec1(v) {}
    explicit ShaderVariant(const sm::vec2& v) : type(VariableType::Vector2), vec2(v) {}
    explicit ShaderVariant(const sm::vec3& v) : type(VariableType::Vector3), vec3(v) {}
    explicit ShaderVariant(const sm::vec4& v) : type(VariableType::Vector4), vec4(v) {}
    explicit ShaderVariant(const sm::mat2& v) : type(VariableType::Matrix2), mat2(v) {}
    explicit ShaderVariant(const sm::mat3& v) : type(VariableType::Matrix3), mat3(v) {}
    explicit ShaderVariant(const sm::mat4& v) : type(VariableType::Matrix4), mat4(v) {}

    VariableType type;

    union
    {
        float    vec1;
        sm::vec2 vec2;
        sm::vec3 vec3;
        sm::vec4 vec4;
        sm::mat2 mat2;
        sm::mat3 mat3;
        sm::mat4 mat4;
    };

}; // ShaderVariant

}