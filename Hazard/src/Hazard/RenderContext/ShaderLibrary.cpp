
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

		DescriptorSetLayout setLayout = { { SHADER_STAGE_ALL_GRAPHICS, "u_Camera", 0, DESCRIPTOR_TYPE_UNIFORM_BUFFER },
										  { SHADER_STAGE_FRAGMENT_BIT, "u_RadianceMap", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE },
										  { SHADER_STAGE_FRAGMENT_BIT, "u_IrradianceMap", 2, DESCRIPTOR_TYPE_SAMPLER_CUBE },
										  { SHADER_STAGE_FRAGMENT_BIT, "u_BRDFLut", 3, DESCRIPTOR_TYPE_SAMPLER_2D }
		};
		{
			BufferLayout layout = LineVertex::Layout();
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Debug/LineShader.glsl");

			PipelineSpecification specs = {
				.DebugName = "Pipeline LineShader",
				.Usage = PipelineUsage::GraphicsBit,
				.pBufferLayout = &layout,
				.Flags = PIPELINE_PRIMITIVE_TOPOLOGY_LINE_LIST | PIPELINE_DRAW_LINE | PIPELINE_DEPTH_WRITE | PIPELINE_DEPTH_TEST,
				.Shaders = asset->ShaderCode[api],
				.SetLayouts = { setLayout },
			};

			s_LoadedPipelines["LineShader"] = Pipeline::Create(&specs);
		}
		{
			BufferLayout layout = CircleVertex::Layout();
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/2D/CircleShader.glsl");

			PipelineSpecification specs = {
				.DebugName = "Pipeline CircleShader",
				.Usage = PipelineUsage::GraphicsBit,
				.pBufferLayout = &layout,
				.Flags = PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | PIPELINE_DRAW_FILL | PIPELINE_CULL_BACK_FACE | PIPELINE_DEPTH_WRITE | PIPELINE_DEPTH_TEST,
				.Shaders = asset->ShaderCode[api],
			};

			s_LoadedPipelines["CircleShader"] = Pipeline::Create(&specs);
		}
		{
			BufferLayout layout = QuadVertex::Layout();
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/2D/QuadShader.glsl");

			DescriptorSetLayout samplerSet = { { SHADER_STAGE_FRAGMENT_BIT, "u_Textures", 0, 32, DESCRIPTOR_TYPE_SAMPLER_2D} };

			PipelineSpecification specs = {
				.DebugName = "Pipeline QuadShader",
				.Usage = PipelineUsage::GraphicsBit,
				.pBufferLayout = &layout,
				.Flags = PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | PIPELINE_DRAW_FILL | PIPELINE_CULL_BACK_FACE | PIPELINE_DEPTH_WRITE | PIPELINE_DEPTH_TEST,
				.Shaders = asset->ShaderCode[api],
				.SetLayouts = { setLayout, samplerSet },
			};

			s_LoadedPipelines["QuadShader"] = Pipeline::Create(&specs);
		}
		{
			BufferLayout layout = Vertex3D::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/PBR_Static.glsl");

			DescriptorSetLayout materialLayout = { { SHADER_STAGE_FRAGMENT_BIT, "u_Albedo", 0, 1, DESCRIPTOR_TYPE_SAMPLER_2D } };

			PipelineSpecification specs = {
				.DebugName = "Pipeline PBR_Static",
				.Usage = PipelineUsage::GraphicsBit,
				.pBufferLayout = &layout,
				.Flags = PIPELINE_DRAW_FILL | PIPELINE_CULL_BACK_FACE | PIPELINE_DEPTH_WRITE | PIPELINE_DEPTH_TEST | PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				.Shaders = asset->ShaderCode[api],
				.SetLayouts = { setLayout, materialLayout },
				.PushConstants = { { "Albedo", SHADER_STAGE_FRAGMENT_BIT, ShaderDataType::Float4, 0 },
								   { "Metalness", SHADER_STAGE_FRAGMENT_BIT, ShaderDataType::Float, sizeof(float) * 4 },
								   { "Roughness", SHADER_STAGE_FRAGMENT_BIT, ShaderDataType::Float, sizeof(float) * 5 } 
								 },
			};

			s_LoadedPipelines["PBR_Static"] = Pipeline::Create(&specs);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Skybox.glsl");

			PipelineSpecification specs = {
				.DebugName = "Pipeline Skybox",
				.Usage = PipelineUsage::GraphicsBit,
				.Flags = PIPELINE_DRAW_FILL | PIPELINE_DEPTH_TEST | PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				.DepthOperator = DepthOp::LessOrEqual,
				.Shaders = asset->ShaderCode[api],
				.SetLayouts = { setLayout },
				.PushConstants = { { "LodLevel", SHADER_STAGE_FRAGMENT_BIT, ShaderDataType::Float, 0 },
								   { "Intensity", SHADER_STAGE_FRAGMENT_BIT, ShaderDataType::Float, sizeof(float) } 
								 },
			};

			s_LoadedPipelines["Skybox"] = Pipeline::Create(&specs);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Compute/EquirectangularToCubemap.glsl");

			DescriptorSetLayout layout = {
				{ SHADER_STAGE_COMPUTE_BIT, "o_CubeMap", 0, DESCRIPTOR_TYPE_STORAGE_IMAGE },
				{ SHADER_STAGE_COMPUTE_BIT, "u_EquirectangularTexture", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE }
			};

			PipelineSpecification specs = {
				.DebugName = "Pipeline EquirectangularToCubemap",
				.Usage = PipelineUsage::ComputeBit,
				.Shaders = asset->ShaderCode[api],
				.SetLayouts = { layout },
			};

			s_LoadedPipelines["EquirectangularToCubemap"] = Pipeline::Create(&specs);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Compute/EnvironmentIrradiance.glsl");

			DescriptorSetLayout layout = {
				{ SHADER_STAGE_COMPUTE_BIT, "o_IrradianceMap", 0, DESCRIPTOR_TYPE_STORAGE_IMAGE },
				{ SHADER_STAGE_COMPUTE_BIT, "u_RadianceMap", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE }
			};

			PipelineSpecification specs = {
				.DebugName = "Pipeline EnvironmentIrradiance",
				.Usage = PipelineUsage::ComputeBit,
				.Shaders = asset->ShaderCode[api],
				.SetLayouts = { layout },
				.PushConstants = { { "Samples", SHADER_STAGE_COMPUTE_BIT, ShaderDataType::UInt, 0 } },
			};

			s_LoadedPipelines["EnvironmentIrradiance"] = Pipeline::Create(&specs);
		}
	}

	Ref<HazardRenderer::Pipeline> ShaderLibrary::GetPipeline(const std::string& name)
	{
		if (s_LoadedPipelines.find(name) == s_LoadedPipelines.end())
			return nullptr;

		return s_LoadedPipelines[name];
	}
}
