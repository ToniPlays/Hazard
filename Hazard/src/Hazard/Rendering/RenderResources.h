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
		Ref<HazardRenderer::DescriptorSet> WorldDescriptor;
		Ref<HazardRenderer::DescriptorSet> SkyboxDescriptor;
		Ref<HazardRenderer::GPUBuffer> CameraUniformBuffer;

		AssetHandle SkyboxMaterialHandle;
		AssetHandle PBRMaterialHandle;

		AssetHandle WhiteTextureHandle;
		AssetHandle BRDFLutHandle;

		Ref<Texture2DAsset> BRDFLut;
		Ref<HazardRenderer::CubemapTexture> BlackCubemap;
		Ref<HazardRenderer::CubemapTexture> WhiteCubemap;
		Ref<HazardRenderer::Sampler> DefaultImageSampler;

		void Initialize(Ref<HazardRenderer::RenderPass> renderPass)
		{
			DescriptorSetLayout skyboxMaterialLayout = { 
				{ SHADER_STAGE_VERTEX_BIT, "u_Camera", 0, DESCRIPTOR_TYPE_UNIFORM_BUFFER },
				{ SHADER_STAGE_FRAGMENT_BIT, "u_CubeMap", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE } 
			};

			DescriptorSetCreateInfo skyboxSetSpec = {};
			skyboxSetSpec.DebugName = "Skybox descriptor";
			skyboxSetSpec.Set = 0;
			skyboxSetSpec.pLayout = &skyboxMaterialLayout;

			SkyboxDescriptor = DescriptorSet::Create(&skyboxSetSpec);

			Ref<Material> skyboxMaterial = Ref<Material>::Create(ShaderLibrary::GetPipelineAssetHandle("Skybox"), DescriptorSetLayout());
			SkyboxMaterialHandle = AssetManager::CreateMemoryOnly(AssetType::Material, skyboxMaterial);

			DescriptorSetLayout layout = { { SHADER_STAGE_VERTEX_BIT, "u_Camera", 0, DESCRIPTOR_TYPE_UNIFORM_BUFFER },
										   { SHADER_STAGE_FRAGMENT_BIT, "u_RadianceMap", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE },
										   { SHADER_STAGE_FRAGMENT_BIT, "u_IrradianceMap", 2, DESCRIPTOR_TYPE_SAMPLER_CUBE },
										   { SHADER_STAGE_FRAGMENT_BIT, "u_BRDFLut", 3, DESCRIPTOR_TYPE_SAMPLER_2D } };

			DescriptorSetCreateInfo setInfo = {};
			setInfo.DebugName = "WorldDescriptor";
			setInfo.Set = 0;
			setInfo.pLayout = &layout;

			WorldDescriptor = DescriptorSet::Create(&setInfo);

			BufferCreateInfo cameraUBO = {};
			cameraUBO.Name = "Camera";
			cameraUBO.Size = sizeof(CameraData);
			cameraUBO.UsageFlags = BUFFER_USAGE_UNIFORM_BUFFER_BIT | BUFFER_USAGE_DYNAMIC;

			CameraUniformBuffer = GPUBuffer::Create(&cameraUBO);
			WorldDescriptor->Write(0, CameraUniformBuffer, true);

			auto& resources = Application::GetModule<RenderContextManager>().GetDefaultResources();
			Ref<AssetPointer> asset = AssetPointer::Create(resources.WhiteTexture, AssetType::Image);

			WhiteTextureHandle = AssetManager::CreateMemoryOnly(AssetType::Image, asset);
			BRDFLutHandle = AssetManager::GetHandleFromKey("BRDF_LUT.tga");
			BRDFLut = AssetManager::GetAsset<Texture2DAsset>(BRDFLutHandle);

			Ref<Material> defaultMaterial = Ref<Material>::Create(ShaderLibrary::GetPipelineAssetHandle("PBR_Static"), DescriptorSetLayout());
			PBRMaterialHandle = AssetManager::CreateMemoryOnly(AssetType::Material, defaultMaterial);

			CubemapTextureCreateInfo blackCubemap = {};
			blackCubemap.DebugName = "BlackCubemap";
			blackCubemap.Usage = ImageUsage::Texture;
			blackCubemap.Width = 1;
			blackCubemap.Height = 1;
			blackCubemap.Format = ImageFormat::RGBA;
			blackCubemap.GenerateMips = false;

			BlackCubemap = CubemapTexture::Create(&blackCubemap);

			Buffer data;
			data.Allocate(6 * sizeof(uint32_t));
			data.Initialize(0xFF);

			CubemapTextureCreateInfo whiteCubemap = {};
			whiteCubemap.DebugName = "WhiteCubemap";
			whiteCubemap.Usage = ImageUsage::Texture;
			whiteCubemap.Width = 1;
			whiteCubemap.Height = 1;
			whiteCubemap.Data = data;
			whiteCubemap.Format = ImageFormat::RGBA;
			whiteCubemap.GenerateMips = false;

			WhiteCubemap = CubemapTexture::Create(&whiteCubemap);
			data.Release();
			
			SamplerCreateInfo samplerInfo = {};
			samplerInfo.DebugName = "DefaultImageSampler";
			samplerInfo.MinFilter = FilterMode::Linear;
			samplerInfo.MagFilter = FilterMode::Linear;
			samplerInfo.Wrapping = ImageWrap::Repeat;

			DefaultImageSampler = Sampler::Create(&samplerInfo);

			WorldDescriptor->Write(1, 0, WhiteCubemap, DefaultImageSampler, true);
			WorldDescriptor->Write(2, 0, WhiteCubemap, DefaultImageSampler, true);
			WorldDescriptor->Write(3, 0, BRDFLut->GetSourceImageAsset()->Value.As<Image2D>(), DefaultImageSampler, true);

			SkyboxDescriptor->Write(0, CameraUniformBuffer, true);
		}
	};
}
