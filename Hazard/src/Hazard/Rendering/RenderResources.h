#pragma once

#include "HazardRendererCore.h"
#include "Mesh/Mesh.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/ShaderLibrary.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include "Hazard/RenderContext/Texture2D.h"
#include "Mesh/Material.h"

namespace Hazard
{
	struct CameraData
	{
		glm::mat4 ViewProjection;
		glm::mat4 InverseViewProjection;
		glm::mat4 Projection;
		glm::mat4 InverseProjection;
		glm::mat4 View;
		glm::mat4 InverseView;
		glm::vec2 NDCToViewMul;
		glm::vec2 NDCToViewAdd;
		glm::vec2 DepthUnpack;
		glm::vec2 CameraTanHalfFOV;
		glm::vec4 Position;
		glm::vec4 Unused2;
	};

	struct DirectionalLight
	{
		//W component not used
		glm::vec4 Direction;
		//Alpha is intensity
		glm::vec4 Color;
	};

	struct LightingData
	{
		int DirectionLightCount;
		int PointLightCount;
		float SkyLightIntensity;
		float EnvironmentLod;
		DirectionalLight Lights[8];
	};
	struct UtilityUniformData
	{
		glm::vec3 CameraPos;
		float someRandomStuff;
		uint32_t Flags = 0;
	};

	struct InstanceTransform
	{
		glm::vec4 MRow0;
		glm::vec4 MRow1;
		glm::vec4 MRow2;
	};

	using namespace HazardRenderer;
	struct RenderResources
	{
		Ref<DescriptorSet> WorldDescriptor;
		Ref<DescriptorSet> SkyboxDescriptor;
		Ref<GPUBuffer> CameraUniformBuffer;

		AssetHandle WhiteTextureHandle;
		AssetHandle BRDFLutHandle;

		Ref<Texture2DAsset> BRDFLut;
		Ref<CubemapTexture> BlackCubemap;
		Ref<CubemapTexture> WhiteCubemap;
		Ref<Sampler> DefaultImageSampler;

		void Initialize(Ref<HazardRenderer::RenderPass> renderPass)
		{
			DescriptorSetLayout skyboxMaterialLayout = { 
				{ SHADER_STAGE_VERTEX_BIT, "u_Camera", 0, DESCRIPTOR_TYPE_UNIFORM_BUFFER },
				{ SHADER_STAGE_FRAGMENT_BIT, "u_CubeMap", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE } 
			};

			DescriptorSetCreateInfo skyboxSetSpec = {
				.DebugName = "Skybox descriptor",
				.Set = 0,
				.pLayout = &skyboxMaterialLayout,
			};

			SkyboxDescriptor = DescriptorSet::Create(&skyboxSetSpec);

			DescriptorSetLayout layout = { { SHADER_STAGE_VERTEX_BIT, "u_Camera", 0, DESCRIPTOR_TYPE_UNIFORM_BUFFER },
										   { SHADER_STAGE_FRAGMENT_BIT, "u_RadianceMap", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE },
										   { SHADER_STAGE_FRAGMENT_BIT, "u_IrradianceMap", 2, DESCRIPTOR_TYPE_SAMPLER_CUBE },
										   { SHADER_STAGE_FRAGMENT_BIT, "u_BRDFLut", 3, DESCRIPTOR_TYPE_SAMPLER_2D } };

			DescriptorSetCreateInfo setInfo = {
				.DebugName = "WorldDescriptor",
				.Set = 0,
				.pLayout = &layout,
			};

			WorldDescriptor = DescriptorSet::Create(&setInfo);

			BufferCreateInfo cameraUBO = {
				.Name = "Camera",
				.UsageFlags = BUFFER_USAGE_UNIFORM_BUFFER_BIT | BUFFER_USAGE_DYNAMIC,
				.Size = sizeof(CameraData),
			};

			CameraUniformBuffer = GPUBuffer::Create(&cameraUBO);
			WorldDescriptor->Write(0, CameraUniformBuffer, true);

			auto& resources = Application::GetModule<RenderContextManager>().GetDefaultResources();

			CubemapTextureCreateInfo blackCubemap = {
				.DebugName = "BlackCubemap",
				.Width = 1,
				.Height = 1,
				.MaxMips = 1,
				.Usage = ImageUsage::Texture,
				.Format = ImageFormat::RGBA,
			};

			BlackCubemap = CubemapTexture::Create(&blackCubemap);

			Buffer data;
			data.Allocate(6 * sizeof(uint32_t));
			data.Initialize(0xFF);

			CubemapTextureCreateInfo whiteCubemap = {
				.DebugName = "WhiteCubemap",
				.Width = 1,
				.Height = 1,
				.Usage = ImageUsage::Texture,
				.Format = ImageFormat::RGBA,
				.Data = data,
			};

			WhiteCubemap = CubemapTexture::Create(&whiteCubemap);
			data.Release();
			
			SamplerCreateInfo samplerInfo = {
				.DebugName = "DefaultImageSampler",
				.MinFilter = FilterMode::LinearMip,
				.MagFilter = FilterMode::LinearMip,
				.Wrapping = ImageWrap::Repeat,
			};

			DefaultImageSampler = Sampler::Create(&samplerInfo);
			SkyboxDescriptor->Write(0, CameraUniformBuffer, true);
		}
	};
}
