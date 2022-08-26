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
	};

	struct DirectionalLight {
		//W component not used
		glm::vec4 Direction;
		//Alpha is intensity
		glm::vec4 Color;
	};

	struct LightingData
	{
		int DirectionLightCount;
		int PointLightCount;
		int Padding;
		int Padding2;
		DirectionalLight Lights[16];
	};
	struct EnvironmentUBO 
	{

	};


	using namespace HazardRenderer;
	struct RenderResources
	{
		Ref<HazardRenderer::UniformBuffer> CameraUniformBuffer;
		Ref<HazardRenderer::UniformBuffer> EnvironmentBuffer;
		Ref<HazardRenderer::UniformBuffer> LightUniformBuffer;
		Ref<HazardRenderer::UniformBuffer> ModelUniformBuffer;
		Ref<HazardRenderer::Pipeline> CompositePipeline;
		Ref<HazardRenderer::Pipeline> SkyboxPipeline;

		void Initialize(Ref<HazardRenderer::RenderPass> renderPass)
		{
			{
				UniformBufferCreateInfo cameraUBO = {};
				cameraUBO.Name = "Camera";
				cameraUBO.Binding = 0;
				cameraUBO.Size = sizeof(CameraData);
				cameraUBO.Usage = BufferUsage::DynamicDraw;

				CameraUniformBuffer = UniformBuffer::Create(&cameraUBO);

				UniformBufferCreateInfo environmentUbo = {};
				environmentUbo.Name = "Environment";
				environmentUbo.Binding = 1;
				environmentUbo.Size = sizeof(EnvironmentUBO);
				environmentUbo.Usage = BufferUsage::DynamicDraw;

				EnvironmentBuffer = UniformBuffer::Create(&environmentUbo);

				UniformBufferCreateInfo lightUBO = {};
				lightUBO.Name = "Lights";
				lightUBO.Binding = 2;
				lightUBO.Size = sizeof(LightingData);
				lightUBO.Usage = BufferUsage::DynamicDraw;

				LightUniformBuffer = UniformBuffer::Create(&lightUBO);

				UniformBufferCreateInfo modelUBO = {};
				modelUBO.Name = "Model";
				modelUBO.Binding = 3;
				modelUBO.Size = sizeof(glm::mat4);
				modelUBO.Usage = BufferUsage::DynamicDraw;

				ModelUniformBuffer = UniformBuffer::Create(&modelUBO);
			}
			{
				BufferLayout layout = { {} };

				PipelineSpecification compositeInfo = {};
				compositeInfo.DebugName = "CompositePipeline";
				compositeInfo.pTargetRenderPass = renderPass.Raw();
				compositeInfo.ShaderPath = "Shaders/Composite/SceneComposite.glsl";
				compositeInfo.DrawType = DrawType::Fill;
				compositeInfo.Usage = PipelineUsage::GraphicsBit;
				compositeInfo.CullMode = CullMode::None;
				compositeInfo.IsShared = true;
				compositeInfo.DepthTest = true;
				compositeInfo.pBufferLayout = &layout;

				CompositePipeline = Pipeline::Create(&compositeInfo);

				PipelineSpecification skyboxInfo = {};
				skyboxInfo.DebugName = "Skybox";
				skyboxInfo.pTargetRenderPass = renderPass.Raw();
				skyboxInfo.ShaderPath = "Shaders/Skybox.glsl";
				skyboxInfo.DrawType = DrawType::Fill;
				skyboxInfo.Usage = PipelineUsage::GraphicsBit;
				skyboxInfo.CullMode = CullMode::None;
				skyboxInfo.IsShared = true;
				skyboxInfo.pBufferLayout = &layout;

				SkyboxPipeline = Pipeline::Create(&skyboxInfo);
			}
		}
	};
}