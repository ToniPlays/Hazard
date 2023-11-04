#pragma once

#include "UtilityCore.h"
#include "RenderResources.h"
#include "Renderers/WorldRenderer.h"
#include "Core/Rendering/RenderPass.h"
#include "Core/Rendering/Pipeline.h"
#include "Core/Rendering/GPUBuffer.h"
#include "Environment/EnvironmentMap.h"

#include "AddressableVector.h"

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

	struct EnvironmentData
	{
		Ref<Pipeline> Pipeline;
		Ref<DescriptorSet> MaterialDescriptorSet;
	};

	struct GeometryMesh
	{
		glm::mat4 Transform;
		uint32_t Count;
		Ref<GPUBuffer> VertexBuffer;
		Ref<GPUBuffer> IndexBuffer;
		Ref<Pipeline> Pipeline;
		Ref<DescriptorSet> MaterialDescriptorSet;
	};

	//Draw list for single world context
	struct RendererDrawList
	{
		Ref<WorldRenderer> WorldRenderer;

		EnvironmentData Environment;
		std::vector<GeometryMesh> GeometryMeshes;
		DrawListStat Stats;
	};
}
