
#include <hzrpch.h>
#include "ShaderLibrary.h"

#include "Core/GraphicsContext.h"

#include "Hazard/Rendering/Vertices.h"
#include "Hazard/RenderContext/ShaderAsset.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardRenderer;

	void ShaderLibrary::Init(RenderAPI api)
	{
		HZR_PROFILE_SCOPE();
		HZR_TIMED_FUNCTION();


		DescriptorSetLayout setLayout = { { SHADER_STAGE_VERTEX_BIT, "u_Camera", 0, DESCRIPTOR_TYPE_UNIFORM_BUFFER },
									   { SHADER_STAGE_FRAGMENT_BIT, "u_RadianceMap", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE },
									   { SHADER_STAGE_FRAGMENT_BIT, "u_IrradianceMap", 2, DESCRIPTOR_TYPE_SAMPLER_CUBE },
									   { SHADER_STAGE_FRAGMENT_BIT, "u_BRDFLut", 3, DESCRIPTOR_TYPE_SAMPLER_2D } };

		{
			BufferLayout layout = LineVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("LineShader.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline LineShader";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.pBufferLayout = &layout;
			specs.Flags = PIPELINE_PRIMITIVE_TOPOLOGY_LINE_LIST | PIPELINE_DRAW_LINE | PIPELINE_DEPTH_WRITE;
			specs.Shaders = asset->ShaderCode[api];
			specs.SetLayouts = { setLayout };

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["LineShader"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			BufferLayout layout = CircleVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("CircleShader.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline CircleShader";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.pBufferLayout = &layout;
			specs.Flags = PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | PIPELINE_DRAW_FILL | PIPELINE_CULL_BACK_FACE | PIPELINE_DEPTH_WRITE;
			specs.Shaders = asset->ShaderCode[api];

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["CircleShader"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			BufferLayout layout = QuadVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("QuadShader.glsl");

			DescriptorSetLayout samplerSet = { { SHADER_STAGE_FRAGMENT_BIT, "u_Textures", 0, 32, DESCRIPTOR_TYPE_SAMPLER_2D} };

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline QuadShader";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.pBufferLayout = &layout;
			specs.Flags = PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | PIPELINE_DRAW_FILL | PIPELINE_CULL_BACK_FACE | PIPELINE_DEPTH_WRITE;
			specs.Shaders = asset->ShaderCode[api];
			specs.SetLayouts = { setLayout, samplerSet };
			specs.PushConstants = { { SHADER_STAGE_VERTEX_BIT, sizeof(glm::mat4), 0} };

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["QuadShader"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			BufferLayout layout = Vertex3D::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("PBR_Static.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline PBR_Static";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.pBufferLayout = &layout;
			specs.Flags = PIPELINE_DRAW_FILL | PIPELINE_CULL_BACK_FACE | PIPELINE_DEPTH_WRITE;
			specs.Shaders = asset->ShaderCode[api];

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["PBR_Static"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("Skybox.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline Skybox";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.DepthOperator = DepthOp::LessOrEqual;
			specs.Flags = PIPELINE_DRAW_FILL;
			specs.Shaders = asset->ShaderCode[api];

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["Skybox"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("EquirectangularToCubemap.glsl");

			DescriptorSetLayout layout = {
				{ SHADER_STAGE_COMPUTE_BIT, "o_CubeMap", 0, DESCRIPTOR_TYPE_STORAGE_IMAGE },
				{ SHADER_STAGE_COMPUTE_BIT, "u_EquirectangularTexture", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE }
			};

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline EquirectangularToCubemap";
			specs.Usage = PipelineUsage::ComputeBit;
			specs.Shaders = asset->ShaderCode[api];
			specs.SetLayouts = { layout };

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["EquirectangularToCubemap"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("EnvironmentIrradiance.glsl");

			DescriptorSetLayout layout = {
				{ SHADER_STAGE_COMPUTE_BIT, "o_IrradianceMap", 0, DESCRIPTOR_TYPE_STORAGE_IMAGE }, 
				{ SHADER_STAGE_COMPUTE_BIT, "u_RadianceMap", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE } 
			};

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline EnvironmentIrradiance";
			specs.Usage = PipelineUsage::ComputeBit;
			specs.Shaders = asset->ShaderCode[api];
			specs.SetLayouts = { layout };
			specs.PushConstants = { { SHADER_STAGE_COMPUTE_BIT, sizeof(uint32_t), 0 } };

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["EnvironmentIrradiance"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
	}

	AssetHandle ShaderLibrary::GetPipelineAssetHandle(const std::string& name)
	{
		if (s_LoadedPipelines.find(name) == s_LoadedPipelines.end())
			return INVALID_ASSET_HANDLE;

		return s_LoadedPipelines[name];
	}
}
