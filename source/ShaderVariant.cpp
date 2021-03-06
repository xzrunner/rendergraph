#include "rendergraph/ShaderVariant.h"
#include "rendergraph/ValueImpl.h"

namespace rendergraph
{

ShaderVariant::ShaderVariant()
    : type(VariableType::Any)
{
    memset(&mat4, 0, sizeof(mat4));
}

ShaderVariant::ShaderVariant(const ShaderVariant& var)
{
    memset(&mat4, 0, sizeof(mat4));

    this->operator=(var);
}

ShaderVariant& ShaderVariant::operator = (const ShaderVariant& var)
{
    type = var.type;
    switch (type)
    {
    case VariableType::Any:
        break;
	case VariableType::RenderTarget:
	case VariableType::Shader:
		p = var.p;
		break;
    case VariableType::Sampler2D:
    case VariableType::SamplerCube:
    case VariableType::Texture:
        p = var.p ?  new TextureVal(*(TextureVal*)(var.p)) : nullptr;
        break;
    case VariableType::String:
        if (p) {
            delete (char*)(p);
            p = nullptr;
        }
        if (var.p) {
            p = _strdup(static_cast<const char*>(var.p));
        }
        break;
    case VariableType::Int:
        i = var.i;
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
    case VariableType::Vec1Array:
        vec1_array = var.vec1_array;
        break;
    case VariableType::Vec2Array:
        vec2_array = var.vec2_array;
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

ShaderVariant::~ShaderVariant()
{
	switch (type)
	{
	case VariableType::Texture:
		delete (TextureVal*)(p);
		break;
    case VariableType::String:
        delete (char*)(p);
        break;
	}
}

}