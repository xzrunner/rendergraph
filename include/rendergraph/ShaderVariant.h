#pragma once

#include "rendergraph/VariableType.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>

#include <vector>

namespace rendergraph
{

class ShaderVariant
{
public:
    ShaderVariant();
    ShaderVariant(const ShaderVariant& var);
    ShaderVariant& operator = (const ShaderVariant& var);
	~ShaderVariant();

    explicit ShaderVariant(VariableType type, int id) : type(type), i(id) {}
    explicit ShaderVariant(const std::string& str) : type(VariableType::String), p(_strdup(str.c_str())) {}
    explicit ShaderVariant(int v) : type(VariableType::Int), i(v) {}
    explicit ShaderVariant(bool v) : type(VariableType::Bool), b(v) {}
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
        int      i;
        bool     b;
        float    vec1;
        sm::vec2 vec2;
        sm::vec3 vec3;
        sm::vec4 vec4;
        sm::mat2 mat2;
        sm::mat3 mat3;
        sm::mat4 mat4;
        const void* p;
    };

    // fixme
    std::vector<float>    vec1_array;
    std::vector<sm::vec2> vec2_array;
    std::vector<sm::vec3> vec3_array;
    std::vector<sm::vec4> vec4_array;

}; // ShaderVariant

}