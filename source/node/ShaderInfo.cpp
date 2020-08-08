#include "rendergraph/node/ShaderInfo.h"
#include "rendergraph/node/ShaderParser.h"

#include <shadertrans/ShaderTrans.h>

#include <spirv-cross/spirv.hpp>
#include <spirv-cross/spirv_glsl.hpp>

namespace
{

enum BaseDataType
{
	None,
	Boolean,
	Int,
	Half,
	Float,
	Double,
	IntArray,
	FloatArray,
	Vector2,
	Vector3,
	Vector4,
	Matrix3,
	Matrix4
};
struct SingleData
{
	BaseDataType type;
	uint32_t size;
};

struct BlockBufferMember
{
	BaseDataType type;
	std::string name;
	uint32_t size;
	uint32_t offset;
};

struct BlockBuffer
{
	std::string name;
	std::vector<BlockBufferMember> members;
	uint32_t size;
	uint32_t set;
	uint32_t binding;
};

struct Sampler2D
{
	std::string name;
	uint32_t set;
	uint32_t binding;
};
struct StageInput
{
	BaseDataType type;
	std::string name;
	uint32_t size;
	uint32_t offset;
};

struct ProgramInfo
{
	std::vector<BlockBuffer> blockBuffers;
	std::vector<StageInput> stageInputs;
	std::vector<Sampler2D> sampler2Ds;
	uint32_t stageInputsSize;
	uint32_t setCount;
};

}

namespace rendergraph
{
namespace node
{

//void ShaderInfo::GetCodeUniforms(ur::ShaderType type, const std::string& glsl,
//	                             std::vector<rendergraph::Variable>& uniforms, 
//	                             std::set<std::string>& unique_names)
//{
//    shadertrans::ShaderStage stage;
//    switch (type)
//    {
//    case ur::ShaderType::VertexShader:
//        stage = shadertrans::ShaderStage::VertexShader;
//        break;
//    case ur::ShaderType::GeometryShader:
//        stage = shadertrans::ShaderStage::GeometryShader;
//        break;
//    case ur::ShaderType::FragmentShader:
//        stage = shadertrans::ShaderStage::PixelShader;
//        break;
//    case ur::ShaderType::ComputeShader:
//        stage = shadertrans::ShaderStage::ComputeShader;
//        break;
//    default:
//        assert(0);
//    }
//
//    std::vector<unsigned int> spirv;
//    shadertrans::ShaderTrans::GLSL2SpirV(stage, glsl, spirv);
//
//    spirv_cross::CompilerGLSL compiler(spirv);
//    spirv_cross::ShaderResources resources = compiler.get_shader_resources();
//
//    ProgramInfo* info = new ProgramInfo();
//    uint32_t setCount = 0;
//    //parseUniformBuffers
//    for (auto& resource : resources.uniform_buffers)
//    {
//        BlockBuffer ub;
//        ub.name = compiler.get_name(resource.id);;
//        spirv_cross::SPIRType type = compiler.get_type(resource.base_type_id);
//
//        uint32_t ub_size = 0;
//        uint32_t member_count = type.member_types.size();
//        for (int i = 0; i < member_count; i++)
//        {
//            BlockBufferMember ubm;
//            ubm.name = compiler.get_member_name(resource.base_type_id, i);
//            auto& member_type = compiler.get_type(type.member_types[i]);
//            //ubm.type = parseType(member_type);
//
//            ubm.size = compiler.get_declared_struct_member_size(type, i);
//            ubm.offset = compiler.type_struct_member_offset(type, i);
//
//            ub_size += ubm.size;
//            ub.members.push_back(ubm);
//        }
//        ub.size = ub_size;
//        ub.set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
//        ub.binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
//        if (setCount < ub.set)
//        {
//            setCount = ub.set;
//        }
//        info->blockBuffers.push_back(ub);
//    }
//}

void ShaderInfo::GetCodeUniforms(ur::ShaderType type, const std::string& glsl,
	                             std::vector<rendergraph::Variable>& uniforms, 
	                             std::set<std::string>& unique_names)
{
    ShaderParser parser(glsl);
    parser.Parse();

    auto& unifs = parser.GetUniforms();
    for (auto& u : unifs) {
        if (unique_names.find(u.name) == unique_names.end()) {
            uniforms.push_back(u);
            unique_names.insert(u.name);
        }
    }
}

}
}