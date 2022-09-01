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
		glm::vec4 Unused1;
		glm::vec4 Unused2;
		glm::vec4 Unused3;
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

		void Initialize(Ref<HazardRenderer::RenderPass> renderPass)
		{
			{
				UniformBufferCreateInfo cameraUBO = {};
				cameraUBO.Name = "Camera";
				cameraUBO.Set = 0;
				cameraUBO.Binding = 0;
				cameraUBO.Size = sizeof(CameraData);
				cameraUBO.Usage = BufferUsage::DynamicDraw;

				CameraUniformBuffer = UniformBuffer::Create(&cameraUBO);

				UniformBufferCreateInfo environmentUbo = {};
				environmentUbo.Name = "Environment";
				environmentUbo.Set = 0;
				environmentUbo.Binding = 1;
				environmentUbo.Size = sizeof(EnvironmentUBO);
				environmentUbo.Usage = BufferUsage::DynamicDraw;

				EnvironmentBuffer = UniformBuffer::Create(&environmentUbo);

				UniformBufferCreateInfo lightUBO = {};
				lightUBO.Name = "Lights";
				lightUBO.Set = 0;
				lightUBO.Binding = 2;
				lightUBO.Size = sizeof(LightingData);
				lightUBO.Usage = BufferUsage::DynamicDraw;

				//LightUniformBuffer = UniformBuffer::Create(&lightUBO);

				UniformBufferCreateInfo modelUBO = {};
				modelUBO.Name = "Model";
				modelUBO.Set = 1;
				modelUBO.Binding = 0;
				modelUBO.Size = sizeof(glm::mat4);
				modelUBO.Usage = BufferUsage::DynamicDraw;

				ModelUniformBuffer = UniformBuffer::Create(&modelUBO);
			}
		}
	};
}