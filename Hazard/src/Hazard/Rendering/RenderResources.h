#pragma once

#include "HazardRendererCore.h"
#include "Mesh/Mesh.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/ShaderLibrary.h"

#include "Hazard/RenderContext/RenderContextManager.h"

#include "Hazard/RenderContext/Texture2D.h"

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
		Ref<HazardRenderer::UniformBuffer> CameraUniformBuffer;
		Ref<HazardRenderer::UniformBuffer> UtilityUniformBuffer;
		Ref<HazardRenderer::UniformBuffer> LightUniformBuffer;
        
        Ref<HazardRenderer::VertexBuffer> TransformBuffer;

		Ref<HazardRenderer::Pipeline> SkyboxPipeline;
		Ref<HazardRenderer::Pipeline> PbrPipeline;

        Ref<Texture2DAsset> WhiteTexture;
		Ref<Texture2DAsset> BRDFLut;

		Ref<HazardRenderer::CubemapTexture> BlackCubemap;
		Ref<HazardRenderer::CubemapTexture> WhiteCubemap;

		void Initialize(Ref<HazardRenderer::RenderPass> renderPass)
		{
			{
				UniformBufferCreateInfo utilityUbo = {};
				utilityUbo.Name = "Utility";
				utilityUbo.Set = 0;
				utilityUbo.Binding = 0;
				utilityUbo.Size = sizeof(UtilityUniformData);

				UtilityUniformBuffer = UniformBuffer::Create(&utilityUbo);

				UniformBufferCreateInfo cameraUBO = {};
				cameraUBO.Name = "Camera";
				cameraUBO.Set = 0;
				cameraUBO.Binding = 2;
				cameraUBO.Size = sizeof(CameraData);

				CameraUniformBuffer = UniformBuffer::Create(&cameraUBO);

				UniformBufferCreateInfo lightUBO = {};
				lightUBO.Name = "Lights";
				lightUBO.Set = 0;
				lightUBO.Binding = 1;
				lightUBO.Size = sizeof(LightingData);

				LightUniformBuffer = UniformBuffer::Create(&lightUBO);
                
                VertexBufferCreateInfo transformBufferInfo = {};
                transformBufferInfo.Name = "TransformStorageBuffer";
                transformBufferInfo.Size = sizeof(InstanceTransform) * 10240;
                
                TransformBuffer = VertexBuffer::Create(&transformBufferInfo);
                

				PbrPipeline = ShaderLibrary::GetPipeline("pbr_static");
				PbrPipeline->SetRenderPass(renderPass);
				SkyboxPipeline = ShaderLibrary::GetPipeline("skybox");
				SkyboxPipeline->SetRenderPass(renderPass);

                auto& resources = Application::GetModule<RenderContextManager>().GetDefaultResources();
                
                Ref<AssetPointer> asset = AssetPointer::Create(resources.WhiteTexture, AssetType::Image);
                
                WhiteTexture = Ref<Texture2DAsset>::Create(asset);
                
				BRDFLut = AssetManager::GetAsset<Texture2DAsset>("res/Textures/BRDF_LUT.tga");

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
			}
		}
	};
}
