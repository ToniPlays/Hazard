
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
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Debug/LineShader.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline LineShader";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.pBufferLayout = &layout;
			specs.Flags = PIPELINE_PRIMITIVE_TOPOLOGY_LINE_LIST | PIPELINE_DRAW_LINE | PIPELINE_DEPTH_WRITE | PIPELINE_DEPTH_TEST;
			specs.Shaders = asset->ShaderCode[api];
			specs.SetLayouts = { setLayout };

			s_LoadedPipelines["LineShader"] = Pipeline::Create(&specs);
		}
		{
			BufferLayout layout = CircleVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/2D/CircleShader.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline CircleShader";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.pBufferLayout = &layout;
			specs.Flags = PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | PIPELINE_DRAW_FILL | PIPELINE_CULL_BACK_FACE | PIPELINE_DEPTH_WRITE | PIPELINE_DEPTH_TEST;
			specs.Shaders = asset->ShaderCode[api];

			s_LoadedPipelines["CircleShader"] = Pipeline::Create(&specs);
		}
		{
			BufferLayout layout = QuadVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/2D/QuadShader.glsl");

			DescriptorSetLayout samplerSet = { { SHADER_STAGE_FRAGMENT_BIT, "u_Textures", 0, 32, DESCRIPTOR_TYPE_SAMPLER_2D} };

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline QuadShader";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.pBufferLayout = &layout;
			specs.Flags = PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | PIPELINE_DRAW_FILL | PIPELINE_CULL_BACK_FACE | PIPELINE_DEPTH_WRITE | PIPELINE_DEPTH_TEST;
			specs.Shaders = asset->ShaderCode[api];
			specs.SetLayouts = { setLayout, samplerSet };
			specs.PushConstants = { { SHADER_STAGE_VERTEX_BIT, sizeof(glm::mat4), 0} };

			s_LoadedPipelines["QuadShader"] = Pipeline::Create(&specs);
		}
		{
			BufferLayout layout = Vertex3D::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/PBR_Static.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline PBR_Static";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.pBufferLayout = &layout;
			specs.Flags = PIPELINE_DRAW_FILL | PIPELINE_CULL_BACK_FACE | PIPELINE_DEPTH_WRITE;
			specs.Shaders = asset->ShaderCode[api];

			s_LoadedPipelines["PBR_Static"] = Pipeline::Create(&specs);
		}
		{
			DescriptorSetLayout skyboxLayout = {
				{ SHADER_STAGE_VERTEX_BIT, "u_Camera", 0, DESCRIPTOR_TYPE_UNIFORM_BUFFER },
				{ SHADER_STAGE_FRAGMENT_BIT, "u_CubeMap", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE }
			};

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Skybox.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline Skybox";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.DepthOperator = DepthOp::LessOrEqual;
			specs.Flags = PIPELINE_DRAW_FILL | PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			specs.Shaders = asset->ShaderCode[api];
			specs.SetLayouts = { skyboxLayout };
			specs.PushConstants = { { SHADER_STAGE_FRAGMENT_BIT, sizeof(float) * 2, 0 } };

			s_LoadedPipelines["Skybox"] = Pipeline::Create(&specs);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Compute/EquirectangularToCubemap.glsl");

			DescriptorSetLayout layout = {
				{ SHADER_STAGE_COMPUTE_BIT, "o_CubeMap", 0, DESCRIPTOR_TYPE_STORAGE_IMAGE },
				{ SHADER_STAGE_COMPUTE_BIT, "u_EquirectangularTexture", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE }
			};

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline EquirectangularToCubemap";
			specs.Usage = PipelineUsage::ComputeBit;
			specs.Shaders = asset->ShaderCode[api];
			specs.SetLayouts = { layout };

			s_LoadedPipelines["EquirectangularToCubemap"] = Pipeline::Create(&specs);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Compute/EnvironmentIrradiance.glsl");

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
