#include "rendergraph/node/ShaderInfo.h"

#include <shadertrans/ShaderTrans.h>

#include <spirv-cross/spirv.hpp>
#include <spirv-cross/spirv_glsl.hpp>

namespace
{

rendergraph::VariableType parse_type(const spirv_cross::SPIRType& type)
{
	rendergraph::VariableType ret = rendergraph::VariableType::Any;

	switch (type.basetype)
	{
	case spirv_cross::SPIRType::BaseType::Boolean:
		if (type.columns == 1 && type.vecsize == 1) {
			ret = rendergraph::VariableType::Bool;
		}
		break;

	case spirv_cross::SPIRType::BaseType::Int:
		switch (type.vecsize) 
		{
		case 1: 
			ret = rendergraph::VariableType::Int;
			break;
		case 2: 
			ret = rendergraph::VariableType::Vector2;
			break;
		case 3: 
			ret = rendergraph::VariableType::Vector3;
			break;
		case 4: 
			ret = rendergraph::VariableType::Vector4;
			break;
		}
		break;

	case spirv_cross::SPIRType::BaseType::Float:
		switch (type.columns) 
		{
		case 1:
			switch (type.vecsize) 
			{
			case 1: 
				ret = rendergraph::VariableType::Vector1;
				break;
			case 2: 
				ret = rendergraph::VariableType::Vector2;
				break;
			case 3: 
				ret = rendergraph::VariableType::Vector3;
				break;
			case 4: 
				ret = rendergraph::VariableType::Vector4;
				break;
			}
			break;

		case 2:
			if (type.vecsize == 2) {
				ret = rendergraph::VariableType::Matrix2;
			}
			break;
				
		case 3:
			if (type.vecsize == 3) {
				ret = rendergraph::VariableType::Matrix3;
			}
			break;

		case 4:
			if (type.vecsize == 4) {
				ret = rendergraph::VariableType::Matrix4;
			}
			break;
		}
		break;
	}

	return ret;
}

ur::ImageFormat parse_image_format(spv::ImageFormat fmt)
{
	ur::ImageFormat ret;
	switch (fmt)
	{
	case spv::ImageFormat::ImageFormatRgba32f:
		ret = ur::ImageFormat::rgba32f;
		break;
	case spv::ImageFormat::ImageFormatRgba16f:
		ret = ur::ImageFormat::rgba16f;
		break;
	case spv::ImageFormat::ImageFormatR32f:
		ret = ur::ImageFormat::r32f;
		break;
	case spv::ImageFormat::ImageFormatRgba8:
		ret = ur::ImageFormat::rgba8;
		break;
	case spv::ImageFormat::ImageFormatRgba8Snorm:
		ret = ur::ImageFormat::rgba8_snorm;
		break;
	case spv::ImageFormat::ImageFormatRg32f:
		ret = ur::ImageFormat::rg32f;
		break;
	case spv::ImageFormat::ImageFormatRg16f:
		ret = ur::ImageFormat::rg16f;
		break;
	case spv::ImageFormat::ImageFormatR11fG11fB10f:
		ret = ur::ImageFormat::r11f_g11f_b10f;
		break;
	case spv::ImageFormat::ImageFormatR16f:
		ret = ur::ImageFormat::r16f;
		break;
	case spv::ImageFormat::ImageFormatRgb10A2:
		ret = ur::ImageFormat::rgb10_a2;
		break;
	case spv::ImageFormat::ImageFormatRg16:
		ret = ur::ImageFormat::rg16;
		break;
	case spv::ImageFormat::ImageFormatRg8:
		ret = ur::ImageFormat::rg8;
		break;
	case spv::ImageFormat::ImageFormatR16:
		ret = ur::ImageFormat::r16;
		break;
	case spv::ImageFormat::ImageFormatR8:
		ret = ur::ImageFormat::r8;
		break;
	case spv::ImageFormat::ImageFormatRgba16Snorm:
		ret = ur::ImageFormat::rgba16_snorm;
		break;
	case spv::ImageFormat::ImageFormatRg16Snorm:
		ret = ur::ImageFormat::rg16_snorm;
		break;
	case spv::ImageFormat::ImageFormatRg8Snorm:
		ret = ur::ImageFormat::rg8_snorm;
		break;
	case spv::ImageFormat::ImageFormatR16Snorm:
		ret = ur::ImageFormat::r16_snorm;
		break;
	case spv::ImageFormat::ImageFormatR8Snorm:
		ret = ur::ImageFormat::r8_snorm;
		break;
	case spv::ImageFormat::ImageFormatRgba32i:
		ret = ur::ImageFormat::rgba32i;
		break;
	case spv::ImageFormat::ImageFormatRgba16i:
		ret = ur::ImageFormat::rgba16i;
		break;
	case spv::ImageFormat::ImageFormatRgba8i:
		ret = ur::ImageFormat::rgba8i;
		break;
	case spv::ImageFormat::ImageFormatR32i:
		ret = ur::ImageFormat::r32i;
		break;
	case spv::ImageFormat::ImageFormatRg32i:
		ret = ur::ImageFormat::rg32i;
		break;
	case spv::ImageFormat::ImageFormatRg16i:
		ret = ur::ImageFormat::rg16i;
		break;
	case spv::ImageFormat::ImageFormatRg8i:
		ret = ur::ImageFormat::rg8i;
		break;
	case spv::ImageFormat::ImageFormatR16i:
		ret = ur::ImageFormat::r16i;
		break;
	case spv::ImageFormat::ImageFormatR8i:
		ret = ur::ImageFormat::r8i;
		break;
	case spv::ImageFormat::ImageFormatRgba32ui:
		ret = ur::ImageFormat::rgba32ui;
		break;
	case spv::ImageFormat::ImageFormatRgba16ui:
		ret = ur::ImageFormat::rgba16ui;
		break;
	case spv::ImageFormat::ImageFormatRgba8ui:
		ret = ur::ImageFormat::rgba8ui;
		break;
	case spv::ImageFormat::ImageFormatR32ui:
		ret = ur::ImageFormat::r32ui;
		break;
	case spv::ImageFormat::ImageFormatRgb10a2ui:
		ret = ur::ImageFormat::rgb10_a2ui;
		break;
	case spv::ImageFormat::ImageFormatRg32ui:
		ret = ur::ImageFormat::rg32ui;
		break;
	case spv::ImageFormat::ImageFormatRg16ui:
		ret = ur::ImageFormat::rg16ui;
		break;
	case spv::ImageFormat::ImageFormatRg8ui:
		ret = ur::ImageFormat::rg8ui;
		break;
	case spv::ImageFormat::ImageFormatR16ui:
		ret = ur::ImageFormat::r16ui;
		break;
	case spv::ImageFormat::ImageFormatR8ui:
		ret = ur::ImageFormat::r8ui;
		break;
	default:
		assert(0);
	}
	return ret;
}

ur::AccessType parse_image_access(spv::AccessQualifier access)
{
	ur::AccessType ret;
	switch (access)
	{
	case spv::AccessQualifier::AccessQualifierReadOnly:
		ret = ur::AccessType::ReadOnly;
		break;
	case spv::AccessQualifier::AccessQualifierWriteOnly:
		ret = ur::AccessType::WriteOnly;
		break;
	case spv::AccessQualifier::AccessQualifierReadWrite:
		ret = ur::AccessType::ReadWrite;
		break;
	case spv::AccessQualifier::AccessQualifierMax:
		ret = ur::AccessType::ReadWrite;
		break;
	default:
		assert(0);
	}
	return ret;
}

}

namespace rendergraph
{
namespace node
{

void ShaderInfo::Parse(shadertrans::ShaderStage stage, const std::string& code,
	                   node::Shader::Language lang, std::ostream& out)
{
	if (code.empty()) {
		return;
	}

	auto glsl_code = code;
	if (lang == node::Shader::Language::HLSL) 
	{
		std::vector<unsigned int> spirv;

		shadertrans::ShaderTrans::HLSL2SpirV(stage, code, spirv, out);

		shadertrans::ShaderTrans::SpirV2GLSL(stage, spirv, glsl_code, out);

		lang = node::Shader::Language::GLSL;
	}

	Parse(stage, glsl_code);
}

void ShaderInfo::Parse(shadertrans::ShaderStage stage, const std::string& glsl)
{
    std::vector<unsigned int> spirv;
    shadertrans::ShaderTrans::GLSL2SpirV(stage, glsl, spirv);

    spirv_cross::CompilerGLSL compiler(spirv);
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	// get_work_group_size_specialization_constants
	auto entries = compiler.get_entry_points_and_stages();
	for (auto& e : entries)
	{
		if (e.execution_model == spv::ExecutionModelGLCompute)
		{
			const auto& spv_entry = compiler.get_entry_point(e.name, e.execution_model);
			m_props.insert({ "local_size_x", ShaderVariant(static_cast<int>(spv_entry.workgroup_size.x)) });
			m_props.insert({ "local_size_y", ShaderVariant(static_cast<int>(spv_entry.workgroup_size.y)) });
			m_props.insert({ "local_size_z", ShaderVariant(static_cast<int>(spv_entry.workgroup_size.z)) });
			break;
		}
	}

	// uniforms
    for (auto& resource : resources.uniform_buffers)
    {
        auto ubo_name = compiler.get_name(resource.id);;
        spirv_cross::SPIRType type = compiler.get_type(resource.base_type_id);

        uint32_t member_count = type.member_types.size();
        for (int i = 0; i < member_count; i++)
        {
			Variable var;
            var.name = compiler.get_member_name(resource.base_type_id, i);
			var.type = parse_type(compiler.get_type(type.member_types[i]));

			uint32_t size = compiler.get_declared_struct_member_size(type, i);
			uint32_t offset = compiler.type_struct_member_offset(type, i);

			m_uniforms.push_back(var);
        }

		uint32_t set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
		uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);		
    }

	for (auto& resource : resources.sampled_images)
	{
		Variable var;

		spirv_cross::SPIRType type = compiler.get_type(resource.base_type_id);

		var.name = compiler.get_name(resource.id);
		uint32_t set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
		uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);

		// todo
		var.type = VariableType::Sampler2D;

		m_uniforms.push_back(var);
	}

	for (auto& resource : resources.storage_images)
	{
		ImageUnit img;

		spirv_cross::SPIRType type = compiler.get_type(resource.base_type_id);

		img.name = compiler.get_name(resource.id);
		uint32_t set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
		img.unit = compiler.get_decoration(resource.id, spv::DecorationBinding);
		img.fmt = parse_image_format(type.image.format);
		img.access = parse_image_access(type.image.access);

		m_images.push_back(img);

		Variable unif;
		unif.name = img.name;
		unif.type = VariableType::Sampler2D;
		m_uniforms.push_back(unif);
	}
}

}
}