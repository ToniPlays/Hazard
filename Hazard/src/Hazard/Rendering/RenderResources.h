#pragma once

#include "HazardRendererCore.h"
#include "Mesh/Mesh.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/ShaderLibrary.h"

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

	struct ModelData
	{
		glm::mat4 Transform;
		int Flags;
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
		float randomShit;
		uint32_t Flags = 0;
	};

	using namespace HazardRenderer;
	struct RenderResources
	{
		Ref<HazardRenderer::UniformBuffer> CameraUniformBuffer;
		Ref<HazardRenderer::UniformBuffer> UtilityUniformBuffer;
		Ref<HazardRenderer::UniformBuffer> LightUniformBuffer;
		Ref<HazardRenderer::UniformBuffer> ModelUniformBuffer;

		Ref<HazardRenderer::Pipeline> SkyboxPipeline;
		Ref<HazardRenderer::Pipeline> PbrPipeline;

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
				utilityUbo.Usage = BufferUsage::DynamicDraw;

				UtilityUniformBuffer = UniformBuffer::Create(&utilityUbo);

				UniformBufferCreateInfo cameraUBO = {};
				cameraUBO.Name = "Camera";
				cameraUBO.Set = 0;
				cameraUBO.Binding = 2;
				cameraUBO.Size = sizeof(CameraData);
				cameraUBO.Usage = BufferUsage::DynamicDraw;

				CameraUniformBuffer = UniformBuffer::Create(&cameraUBO);

				UniformBufferCreateInfo lightUBO = {};
				lightUBO.Name = "Lights";
				lightUBO.Set = 0;
				lightUBO.Binding = 1;
				lightUBO.Size = sizeof(LightingData);
				lightUBO.Usage = BufferUsage::DynamicDraw;

				LightUniformBuffer = UniformBuffer::Create(&lightUBO);

				UniformBufferCreateInfo modelUBO = {};
				modelUBO.Name = "Model";
				modelUBO.Set = 1;
				modelUBO.Binding = 0;
				modelUBO.Size = sizeof(ModelData);
				modelUBO.Usage = BufferUsage::DynamicDraw;

				ModelUniformBuffer = UniformBuffer::Create(&modelUBO);

				PbrPipeline = ShaderLibrary::GetPipeline("pbr_static");
				PbrPipeline->SetRenderPass(renderPass);
				SkyboxPipeline = ShaderLibrary::GetPipeline("skybox");
				SkyboxPipeline->SetRenderPass(renderPass);

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
