#pragma once

#include "HazardRendererCore.h"
#include "Mesh/Mesh.h"

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
	};

	struct ModelData
	{
		glm::mat4 Transform;
		float Metalness;
		float Roughness;
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
		float IBLContribution;
		float SkyLightIntensity;
		DirectionalLight Lights[8];
	};
	struct UtilityUniformData 
	{
		glm::vec4 CameraPos;
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

				BufferLayout layout = {};

				PipelineSpecification skyboxSpec = {};
				skyboxSpec.DebugName = "SkyboxPipeline";
				skyboxSpec.ShaderPath = "res/Shaders/Skybox.glsl";
				skyboxSpec.pBufferLayout = &layout;
				skyboxSpec.pTargetRenderPass = renderPass;
				skyboxSpec.DepthTest = true;
				skyboxSpec.DepthWrite = false;
				skyboxSpec.CullMode = CullMode::None;
				skyboxSpec.DepthOperator = DepthOp::LessOrEqual;
				skyboxSpec.Usage = PipelineUsage::GraphicsBit;

				SkyboxPipeline = Pipeline::Create(&skyboxSpec);

				BufferLayout pbrLayout = Vertex3D::Layout();

				PipelineSpecification pbrPipeline = {};
				pbrPipeline.DebugName = "PBRShader";
				pbrPipeline.Usage = PipelineUsage::GraphicsBit;
				pbrPipeline.DrawType = DrawType::Fill;
				pbrPipeline.ShaderPath = "res/Shaders/pbr.glsl";
				pbrPipeline.pTargetRenderPass = renderPass;
				pbrPipeline.pBufferLayout = &pbrLayout;
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