#pragma once

#include "UtilityCore.h"
#include "Renderers/WorldRenderer.h"
#include "Backend/Core/Pipeline/RenderPass.h"
#include "Backend/Core/Pipeline/Pipeline.h"
#include "Environment/EnvironmentMap.h"

namespace Hazard
{
	struct DrawListStat 
	{
		uint64_t QuadCount;
		uint64_t MeshCount;
		uint64_t Vertices;
		uint64_t Indices;
		uint64_t DrawCalls;
	};

	struct RawMesh
	{
		glm::mat4 Transform;
		Ref<HazardRenderer::VertexBuffer> VertexBuffer;
		Ref<HazardRenderer::IndexBuffer> IndexBuffer;
		size_t Count;
	};
	struct PipelineData 
	{
		size_t Count;
	};

	struct EnvironmentData
	{
		Ref<EnvironmentMap> Map;
		glm::vec4 Color;
		float IBLContribution;
		float Intensity;
	};
	struct DirectionalLightSource
	{
		glm::vec3 Direction;
		glm::vec3 Color;
		float Intensity;
	};

	struct RenderingCamera
	{
		std::string DebugName;
		glm::mat4 ViewProjection = glm::mat4(1.0f);
		glm::mat4 Projection = glm::mat4(1.0f);
		glm::mat4 View = glm::mat4(1.0f);
		glm::vec3 Position = glm::vec3(0.0f);
		Ref<HazardRenderer::RenderPass> RenderPass;
		GeometryInclude GeometryFlags = Geometry_All;
	};

	struct RendererDrawList
	{
		Ref<WorldRenderer> WorldRenderer;
		std::unordered_map<HazardRenderer::Pipeline*, std::vector<RawMesh>> MeshList;
		std::unordered_map<EnvironmentMap*, EnvironmentData> Environment;
		std::vector<DirectionalLightSource> DirectionalLights;
		std::unordered_map<HazardRenderer::Pipeline*, std::vector<PipelineData>> Pipelines;

		DrawListStat Stats;
	};
}