#pragma once

#include "HazardRendererCore.h"

namespace Hazard
{
	struct CameraData
	{
		glm::mat4 ViewProjection;
		glm::mat4 Projection;
		glm::mat4 View;
		glm::mat4 InverseViewProjection;
		glm::vec4 Position;
		float ZNear;
		float ZFar;
		glm::vec2 UnUsed1;
		glm::vec4 Unused2;
		glm::vec4 Unused3;
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
		float IBLContribution;
		float SkyLightIntensity;
		DirectionalLight Lights[8];
	};
	struct UtilityUniformData 
	{
		float Time;
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

		Ref<HazardRenderer::CubemapTexture> BlackCubemap;
		Ref<HazardRenderer::CubemapTexture> WhiteCubemap;

		void Initialize(Ref<HazardRenderer::RenderPass> renderPass)
		{
			{
				UniformBufferCreateInfo cameraUBO = {};
				cameraUBO.Name = "Camera";
				cameraUBO.Set = 0;
				cameraUBO.Binding = 2;
				cameraUBO.Size = sizeof(CameraData);
				cameraUBO.Usage = BufferUsage::DynamicDraw;

				CameraUniformBuffer = UniformBuffer::Create(&cameraUBO);

				UniformBufferCreateInfo environmentUbo = {};
				environmentUbo.Name = "UtilityUniform";
				environmentUbo.Set = 0;
				environmentUbo.Binding = 0;
				environmentUbo.Size = sizeof(UtilityUniformData);
				environmentUbo.Usage = BufferUsage::DynamicDraw;

				UtilityUniformBuffer = UniformBuffer::Create(&environmentUbo);

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
				modelUBO.Size = sizeof(glm::mat4);
				modelUBO.Usage = BufferUsage::DynamicDraw;

				ModelUniformBuffer = UniformBuffer::Create(&modelUBO);

				BufferLayout layout = {};

				PipelineSpecification skyboxSpec = {};
				skyboxSpec.DebugName = "SkyboxPipeline";
				skyboxSpec.ShaderPath = "res/Shaders/Skybox.glsl";
				skyboxSpec.pBufferLayout = &layout;
				skyboxSpec.pTargetRenderPass = renderPass;
				skyboxSpec.DepthTest = true;
				skyboxSpec.DepthWrite = false;
				skyboxSpec.DepthOperator = DepthOp::LessOrEqual;
				skyboxSpec.Usage = PipelineUsage::GraphicsBit;

				SkyboxPipeline = Pipeline::Create(&skyboxSpec);

				PipelineSpecification pbrPipeline = {};
				pbrPipeline.DebugName = "PBRShader";
				pbrPipeline.Usage = PipelineUsage::GraphicsBit;
				pbrPipeline.DrawType = DrawType::Fill;
				pbrPipeline.ShaderPath = "res/Shaders/pbr.glsl";
				pbrPipeline.pTargetRenderPass = renderPass;
				pbrPipeline.pBufferLayout = &layout;
				pbrPipeline.DepthTest = true;

				PbrPipeline = Pipeline::Create(&pbrPipeline);

				CubemapTextureCreateInfo blackCubemap = {};
				blackCubemap.DebugName = "BlackCubemap";
				blackCubemap.Usage = ImageUsage::Texture;
				blackCubemap.Width = 1;
				blackCubemap.Height = 1;
				blackCubemap.Format = ImageFormat::RGBA16F;

				BlackCubemap = CubemapTexture::Create(&blackCubemap);

				uint32_t whiteData = 0xFFFFFF;
				Buffer data = Buffer(&whiteData, sizeof(uint32_t));

				CubemapTextureCreateInfo whiteCubemap = {};
				whiteCubemap.DebugName = "WhiteCubemap";
				whiteCubemap.Usage = ImageUsage::Texture;
				whiteCubemap.Width = 1;
				whiteCubemap.Height = 1;
				whiteCubemap.Data = data;
				whiteCubemap.Format = ImageFormat::RGBA16F;

				WhiteCubemap = CubemapTexture::Create(&whiteCubemap);
			}
		}
	};
}