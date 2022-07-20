#pragma once

#include "UtilityCore.h"
#include "WorldRenderer.h"
#include "Backend/Core/Pipeline/RenderPass.h"

namespace Hazard
{
	struct RawMesh {
		Ref<HazardRenderer::VertexBuffer> VertexBuffer;
		Ref<HazardRenderer::IndexBuffer> IndexBuffer;
		uint32_t Count;
	};

	struct EnvironmentData
	{
		float LightIntensity;
	};
	struct LightSource
	{
		glm::mat4 Transform;
		glm::vec3 Color;
		float Intensity;
	};

	struct RenderingCamera
	{
		std::string DebugName;
		glm::mat4 ViewProjection = glm::mat4(1.0f);
		glm::vec3 Position = glm::vec3(0.0f);
		Ref<HazardRenderer::RenderPass> RenderPass;
		GeometryInclude GeometryFlags = Geometry_All;
	};

	struct RendererDrawList
	{
		std::vector<RenderingCamera> RenderingCameras;
		std::vector<EnvironmentData> Environment;
		std::vector<LightSource> LightSource;
		std::unordered_map<HazardRenderer::Pipeline*, std::vector<RawMesh>> Meshes;

	};
}