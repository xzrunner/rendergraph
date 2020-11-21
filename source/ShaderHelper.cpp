#include "rendergraph/ShaderHelper.h"
#include "rendergraph/VariableType.h"
#include "rendergraph/Variable.h"
#include "rendergraph/ShaderVariant.h"

#include <unirender/ShaderProgram.h>
#include <unirender/Uniform.h>

namespace rendergraph
{

void ShaderHelper::SetUniformValue(const std::shared_ptr<ur::ShaderProgram>& prog,
                                   const Variable& k, const ShaderVariant& v)
{
	if (v.type == VariableType::Any) {
		return;
	}

    if (k.count > 1)
    {
        switch (v.type)
        {
        case VariableType::Vec1Array:
		{
			ShaderVariant var;
			var.type = VariableType::Vec1Array;
			var.vec1_array = v.vec1_array;
			SetUniformValue(prog, k.base_name, k.name, var);
		}
            break;
        case VariableType::Vec2Array:
		{
			ShaderVariant var;
			var.type = VariableType::Vec2Array;
			var.vec2_array = v.vec2_array;
			SetUniformValue(prog, k.base_name, k.name, var);
		}
            break;
        case VariableType::Vec3Array:
		{
			ShaderVariant var;
			var.type = VariableType::Vec3Array;
			var.vec3_array = v.vec3_array;
			SetUniformValue(prog, k.base_name, k.name, var);
		}
            break;
        case VariableType::Vec4Array:
		{
			ShaderVariant var;
			var.type = VariableType::Vec4Array;
			var.vec4_array = v.vec4_array;
			SetUniformValue(prog, k.base_name, k.name, var);
		}
            break;
        default:
            assert(0);
        }
    }
    else
    {
        SetUniformValue(prog, k.base_name, k.name, v);
    }
}

void ShaderHelper::SetUniformValue(const std::shared_ptr<ur::ShaderProgram>& prog,
                                   const std::string& base_name, const std::string& name, const ShaderVariant& v)
{
    std::string u_name;
    if (!base_name.empty()) {
        u_name = base_name + ".";
    }
    u_name += name;

    auto uniform = prog->QueryUniform(u_name);
    if (uniform)
    {
	    switch (v.type)
	    {
	    case VariableType::Int:
		    uniform->SetValue(&v.i, 1);
		    break;
	    case VariableType::Bool:
	    {
		    int i = v.b ? 1 : 0;
		    uniform->SetValue(&i, 1);
	    }
	    break;
	    case VariableType::Vector1:
		    uniform->SetValue(&v.vec1, 1);
		    break;
	    case VariableType::Vector2:
		    uniform->SetValue(v.vec2.xy, 2);
		    break;
	    case VariableType::Vector3:
		    uniform->SetValue(v.vec3.xyz, 3);
		    break;
	    case VariableType::Vector4:
		    uniform->SetValue(v.vec4.xyzw, 4);
		    break;
	    case VariableType::Matrix2:
		    uniform->SetValue(v.mat2.x, 2 * 2);
		    break;
	    case VariableType::Matrix3:
		    uniform->SetValue(v.mat3.x, 3 * 3);
		    break;
	    case VariableType::Matrix4:
		    uniform->SetValue(v.mat4.x, 4 * 4);
		    break;
	    case VariableType::Sampler2D:
	    case VariableType::SamplerCube:
		    break;
	    case VariableType::Vec1Array:
            if (!v.vec1_array.empty()) {
                uniform->SetValue(v.vec1_array.data(), v.vec1_array.size());
            }
		    break;
	    case VariableType::Vec2Array:
		    if (!v.vec2_array.empty()) {
			    uniform->SetValue(v.vec2_array[0].xy, v.vec2_array.size() * 2);
		    }
		    break;
	    case VariableType::Vec3Array:
		    if (!v.vec3_array.empty()) {
			    uniform->SetValue(v.vec3_array[0].xyz, v.vec3_array.size() * 3);
		    }
            break;
        case VariableType::Vec4Array:
		    if (!v.vec4_array.empty()) {
			    uniform->SetValue(v.vec4_array[0].xyzw, v.vec4_array.size() * 4);
		    }
            break;
        }
    }
    else
    {
        auto get_unif_name = [&](int idx) -> std::string
        {
            if (!base_name.empty()) {
                return base_name + "[" + std::to_string(idx) + "]." + name;
            } else {
                return name + "[" + std::to_string(idx) + "]";
            }
        };

        switch (v.type)
        {
        case VariableType::Vec1Array:
            for (int i = 0, n = v.vec1_array.size(); i < n; ++i)
            {
                auto uniform = prog->QueryUniform(get_unif_name(i));
                if (uniform) {
                    uniform->SetValue(&v.vec1_array[i], 1);
                }
            }
            break;
        case VariableType::Vec2Array:
            for (int i = 0, n = v.vec2_array.size(); i < n; ++i)
            {
                auto uniform = prog->QueryUniform(get_unif_name(i));
                if (uniform) {
                    uniform->SetValue(v.vec2_array[i].xy, 2);
                }
            }
            break;
        case VariableType::Vec3Array:
            for (int i = 0, n = v.vec3_array.size(); i < n; ++i)
            {
                auto uniform = prog->QueryUniform(get_unif_name(i));
                if (uniform) {
                    uniform->SetValue(v.vec3_array[i].xyz, 3);
                }
            }
            break;
        case VariableType::Vec4Array:
            for (int i = 0, n = v.vec4_array.size(); i < n; ++i)
            {
                auto uniform = prog->QueryUniform(get_unif_name(i));
                if (uniform) {
                    uniform->SetValue(v.vec4_array[i].xyzw, 4);
                }
            }
            break;
        }
    }
}

}