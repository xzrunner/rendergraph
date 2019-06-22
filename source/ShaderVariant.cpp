#include "rendergraph/ShaderVariant.h"

namespace rg
{

ShaderVariant::ShaderVariant()
    : type(VariableType::Any)
{
}

ShaderVariant::ShaderVariant(const ShaderVariant& var)
{
    this->operator=(var);
}

ShaderVariant& ShaderVariant::operator = (const ShaderVariant& var)
{
    type = var.type;
    switch (type)
    {
    case VariableType::Sampler2D:
    case VariableType::SamplerCube:
        id = var.id;
        break;
    case VariableType::Bool:
        b = var.b;
        break;
    case VariableType::Vector1:
        vec1 = var.vec1;
        break;
    case VariableType::Vector2:
        vec2 = var.vec2;
        break;
    case VariableType::Vector3:
        vec3 = var.vec3;
        break;
    case VariableType::Vector4:
        vec4 = var.vec4;
        break;
    case VariableType::Matrix2:
        mat2 = var.mat2;
        break;
    case VariableType::Matrix3:
        mat3 = var.mat3;
        break;
    case VariableType::Matrix4:
        mat4 = var.mat4;
        break;
    case VariableType::Vec3Array:
        vec3_array = var.vec3_array;
        break;
    case VariableType::Vec4Array:
        vec4_array = var.vec4_array;
        break;
    default:
        assert(0);
    }
    return *this;
}

}