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
		glm::vec4 Unused;
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
		Ref<HazardRenderer::GPUBuffer> CameraUniformBuffer;
		Ref<HazardRenderer::GPUBuffer> UtilityUniformBuffer;
		Ref<HazardRenderer::GPUBuffer> LightUniformBuffer;
        
        Ref<HazardRenderer::GPUBuffer> TransformBuffer;

		AssetHandle SkyboxPipelineHandle;
		AssetHandle PBRMaterialHandle;

        AssetHandle WhiteTextureHandle;
		AssetHandle BRDFLut;

		Ref<HazardRenderer::CubemapTexture> BlackCubemap;
		Ref<HazardRenderer::CubemapTexture> WhiteCubemap;

		Ref<HazardRenderer::Sampler> DefaultImageSampler;

		void Initialize(Ref<HazardRenderer::RenderPass> renderPass)
		{
			{
				BufferCreateInfo cameraUBO = {};
				cameraUBO.Name = "Camera";
				cameraUBO.Size = sizeof(CameraData);
				cameraUBO.UsageFlags = BUFFER_USAGE_UNIFORM_BUFFER_BIT | BUFFER_USAGE_DYNAMIC;

				CameraUniformBuffer = GPUBuffer::Create(&cameraUBO);
				SkyboxPipelineHandle = AssetManager::GetHandleFromKey("Skybox.glsl");

                auto& resources = Application::GetModule<RenderContextManager>().GetDefaultResources();
                Ref<AssetPointer> asset = AssetPointer::Create(resources.WhiteTexture, AssetType::Image);
                
				WhiteTextureHandle = AssetManager::CreateMemoryOnly(AssetType::Image, asset);
				BRDFLut = AssetManager::GetHandleFromKey("BRDF_LUT.tga");


				Ref<Material> defaultMaterial = Ref<Material>::Create(ShaderLibrary::GetPipelineAssetHandle("PBR_Static"));
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

				WhiteCubemap = CubemapTexture::Create(&whiteCubemap);
				data.Release();

				SamplerCreateInfo samplerInfo = {};
				samplerInfo.DebugName = "DefaultImageSampler";
				samplerInfo.MinFilter = FilterMode::Linear;
				samplerInfo.MagFilter = FilterMode::Linear;
				samplerInfo.Wrapping = ImageWrap::Repeat;

				DefaultImageSampler = Sampler::Create(&samplerInfo);
			}
		}
	};
}
