#include "rendergraph/node/Shader.h"
#include "rendergraph/node/ShaderParser.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/ShaderInfo.h"
#include "rendergraph/Evaluator.h"
#include "rendergraph/Variable.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/ValueImpl.h"

#include <unirender/Device.h>
#include <unirender/Context.h>
#include <unirender/ShaderProgram.h>
#include <unirender/Uniform.h>
#include <painting0/ModelMatUpdater.h>
#include <painting0/CamPosUpdater.h>
#include <cpputil/StringHelper.h>
#include <shadertrans/ShaderTrans.h>

namespace rendergraph
{
namespace node
{

void Shader::SetCodes(const std::string& vert, const std::string& frag)
{
    if (m_vert == vert && m_frag == frag) {
        return;
    }

    m_vert = vert;
    m_frag = frag;

    m_prog.reset();

    std::vector<Variable> uniforms;
    std::set<std::string> names;
    ShaderInfo::GetCodeUniforms(ur::ShaderType::VertexShader, m_vert, m_lang, uniforms, names);
    ShaderInfo::GetCodeUniforms(ur::ShaderType::FragmentShader, m_frag, m_lang, uniforms, names);

    std::vector<Port> imports;
    imports.reserve(uniforms.size());
    for (auto& u : uniforms)
    {
        dag::Node<rendergraph::Variable>::Port port;

        port.var.type = u;

        for (auto& p : m_imports) {
            if (p.var.type.type == u.type &&
                p.var.type.name == u.name) {
                port.conns = p.conns;
                break;
            }
        }

        imports.push_back(port);
    }
    m_imports = imports;
}

void Shader::Bind(RenderContext& rc)
{
    if (!m_prog && rc.ur_dev) {
        Init(*rc.ur_dev);
    }
    if (!m_prog || !m_prog->CheckStatus()) {
        return;
    }

    rc.ur_ds.program = m_prog;
    for (auto& ip : m_imports)
    {
        uint32_t flags = 0;
        auto val = Evaluator::Calc(rc, ip, ip.var.type.type, ip.var.type.count, flags);
        SetUniformValue(ip.var.type, val);

        if (flags & Evaluator::FLAG_MODEL_MAT) {
            auto up = std::make_shared<pt0::ModelMatUpdater>(*m_prog, ip.var.type.name);
            m_prog->AddUniformUpdater(up);
        }
        if (flags & Evaluator::FLAG_CAMERA_POS) {
            auto up = std::make_shared<pt0::CamPosUpdater>(*m_prog, ip.var.type.name);
            m_prog->AddUniformUpdater(up);
        }
        if (val.type == VariableType::Texture && val.p)
        {
            const int slot = m_prog->QueryTexSlot(ip.var.type.name);
            SetUniformValue(rc.ur_dev, ip.var.type.name, ShaderVariant(slot));
            auto tex = reinterpret_cast<const TextureVal*>(val.p);
			if (tex->texture) {
				rc.ur_ctx->SetTexture(slot, tex->texture);
			}
			if (tex->sampler) {
				rc.ur_ctx->SetTextureSampler(slot, tex->sampler);
			}
        }
    }
}

std::shared_ptr<ur::ShaderProgram> Shader::GetShader(const RenderContext& rc)
{
    if (!m_prog && rc.ur_dev) {
        Init(*rc.ur_dev);
    }
    return m_prog;
}

void Shader::SetUniformValue(const ur::Device* dev, const std::string& key,
                             const ShaderVariant& val)
{
    if (!m_prog && dev) {
        Init(*dev);
    }
    if (!m_prog || !m_prog->CheckStatus()) {
        return;
    }

    int key_idx = -1;
    for (int i = 0, n = m_imports.size(); i < n; ++i) {
        if (m_imports[i].var.type.name == key) {
            key_idx = i;
            break;
        }
    }

    if (key_idx < 0) {
        return;
    }

    SetUniformValue(m_imports[key_idx].var.type, val);
}

void Shader::Init(const ur::Device& dev)
{
    if (m_prog || m_vert.empty() || m_frag.empty()) {
        return;
    }

    std::vector<unsigned int> vs, fs;
    switch (m_lang)
    {
    case Language::GLSL:
        shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::VertexShader, m_vert, vs);
        shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::PixelShader, m_frag, fs);
        break;
    case Language::HLSL:
        shadertrans::ShaderTrans::HLSL2SpirV(shadertrans::ShaderStage::VertexShader, m_vert, vs);
        shadertrans::ShaderTrans::HLSL2SpirV(shadertrans::ShaderStage::PixelShader, m_frag, fs);
        break;
    default:
        assert(0);
    }
    m_prog = dev.CreateShaderProgram(vs, fs);
}

void Shader::SetUniformValue(const Variable& k, const ShaderVariant& v)
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
			SetUniformValue(k.user_type, k.name, var);
		}
            break;
        case VariableType::Vec2Array:
		{
			ShaderVariant var;
			var.type = VariableType::Vec2Array;
			var.vec2_array = v.vec2_array;
			SetUniformValue(k.user_type, k.name, var);
		}
            break;
        case VariableType::Vec3Array:
		{
			ShaderVariant var;
			var.type = VariableType::Vec3Array;
			var.vec3_array = v.vec3_array;
			SetUniformValue(k.user_type, k.name, var);
		}
            break;
        case VariableType::Vec4Array:
		{
			ShaderVariant var;
			var.type = VariableType::Vec4Array;
			var.vec4_array = v.vec4_array;
			SetUniformValue(k.user_type, k.name, var);
		}
            break;
        default:
            assert(0);
        }
    }
    else
    {
        SetUniformValue(k.user_type, k.name, v);
    }
}

void Shader::SetUniformValue(const std::string& base_name, const std::string& name, const ShaderVariant& v)
{
    std::string u_name;
    if (!base_name.empty()) {
        u_name = base_name + ".";
    }
    u_name += name;

    auto uniform = m_prog->QueryUniform(u_name);
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
                auto uniform = m_prog->QueryUniform(get_unif_name(i));
                if (uniform) {
                    uniform->SetValue(&v.vec1_array[i], 1);
                }
            }
            break;
        case VariableType::Vec2Array:
            for (int i = 0, n = v.vec2_array.size(); i < n; ++i)
            {
                auto uniform = m_prog->QueryUniform(get_unif_name(i));
                if (uniform) {
                    uniform->SetValue(v.vec2_array[i].xy, 2);
                }
            }
            break;
        case VariableType::Vec3Array:
            for (int i = 0, n = v.vec3_array.size(); i < n; ++i)
            {
                auto uniform = m_prog->QueryUniform(get_unif_name(i));
                if (uniform) {
                    uniform->SetValue(v.vec3_array[i].xyz, 3);
                }
            }
            break;
        case VariableType::Vec4Array:
            for (int i = 0, n = v.vec4_array.size(); i < n; ++i)
            {
                auto uniform = m_prog->QueryUniform(get_unif_name(i));
                if (uniform) {
                    uniform->SetValue(v.vec4_array[i].xyzw, 4);
                }
            }
            break;
        }
    }
}

}
}