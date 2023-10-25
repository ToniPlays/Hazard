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

	//Draw list for single world context
	struct RendererDrawList
	{
		Ref<WorldRenderer> WorldRenderer;
		DrawListStat Stats;

		std::vector<HazardRenderer::GraphInstruction> GeometryPassInstructions;
		std::vector<HazardRenderer::GraphInstruction> SkyboxInstructions;
		//Resources
		AddressableVector<void*, HazardRenderer::ResourceReference> Buffers;
		AddressableVector<void*, HazardRenderer::ResourceReference> Pipelines;
		std::vector<HazardRenderer::ResourceReference> PushConstantBuffers;

		std::vector<Buffer> PushConstantData;
		std::vector<Ref<Sampler>> Samplers;
		std::vector<Ref<Texture>> Textures;
	};
}
