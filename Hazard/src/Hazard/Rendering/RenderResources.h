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

	using namespace HazardRenderer;
	struct RenderResources
	{
		Ref<GPUBuffer> CameraUniformBuffer;

		Ref<Cubemap> BlackCubemap;
		Ref<Cubemap> WhiteCubemap;
		Ref<Sampler> DefaultImageSampler;

		void Initialize(Ref<HazardRenderer::RenderPass> renderPass)
		{

			BufferCreateInfo cameraUBO = {
				.Name = "Camera",
				.UsageFlags = BUFFER_USAGE_UNIFORM_BUFFER_BIT | BUFFER_USAGE_DYNAMIC,
				.Size = sizeof(CameraData) * 32,
			};

			CameraUniformBuffer = GPUBuffer::Create(&cameraUBO);

			auto& resources = Application::Get().GetModule<RenderContextManager>().GetWindow().GetContext()->GetDefaultResources();

			CubemapCreateInfo blackCubemap = {
				.DebugName = "BlackCubemap",
				.Width = 1,
				.Height = 1,
				.MaxMips = 1,
				.Usage = ImageUsage::Texture,
				.Format = ImageFormat::RGBA,
			};

			BlackCubemap = Cubemap::Create(&blackCubemap);

			Buffer data;
			data.Allocate(6 * sizeof(uint32_t));
			data.Initialize(0xFF);

			CubemapCreateInfo whiteCubemap = {
				.DebugName = "WhiteCubemap",
				.Width = 1,
				.Height = 1,
				.MaxMips = 1,
				.Usage = ImageUsage::Texture,
				.Format = ImageFormat::RGBA,
				.Data = data,
			};

			WhiteCubemap = Cubemap::Create(&whiteCubemap);
			data.Release();
			
			SamplerCreateInfo samplerInfo = {
				.DebugName = "DefaultImageSampler",
				.MinFilter = FilterMode::LinearMip,
				.MagFilter = FilterMode::LinearMip,
				.Wrapping = ImageWrap::Repeat,
			};

			DefaultImageSampler = Sampler::Create(&samplerInfo);
		}
	};
}
