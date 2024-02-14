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
		struct SkyboxConstants
		{
			float LodLevel;
			float Intensity;
		};

		Ref<Pipeline> Pipeline;
		Ref<GPUBuffer> CameraBuffer;
		Ref<DescriptorSet> DescriptorSet;
		SkyboxConstants Constants;
	};

	struct MeshData
	{
		Ref<GPUBuffer> VertexBuffer;
		Ref<GPUBuffer> IndexBuffer;
		Ref<Material> Material;
		uint32_t Count;
	};

	struct MeshTransform
	{
		glm::vec4 MRow[3];

		MeshTransform(const glm::mat4& transform)
		{
			MRow[0] = {transform[0][0], transform[1][0], transform[2][0], transform[3][0]};
			MRow[1] = {transform[0][1], transform[1][1], transform[2][1], transform[3][1]};
			MRow[2] = {transform[0][2], transform[1][2], transform[2][2], transform[3][2]};
		}
	};

	struct MeshKey
	{
		Ref<GPUBuffer> VertexBuffer;
		Ref<GPUBuffer> IndexBuffer;
		Ref<Material> Material;
		uint32_t Count;

		bool operator<(const MeshKey& other) const
		{
			if (VertexBuffer < other.VertexBuffer)
				return true;
			if (VertexBuffer > other.VertexBuffer)
				return false;

			if (IndexBuffer < other.IndexBuffer)
				return true;
			if (IndexBuffer > other.IndexBuffer)
				return false;

			if (Material < other.Material)
				return true;
			if (Material > other.Material)
				return false;

			return Count < other.Count;
		}
	};

	//Draw list for single world context
	struct RendererDrawList
	{
		Ref<WorldRenderer> WorldRenderer;
		EnvironmentData Environment;

		std::map<MeshKey, MeshData> MeshInstances;
		std::map<MeshKey, std::vector<MeshTransform>> Transforms;

		DrawListStat Stats;
	};
}
