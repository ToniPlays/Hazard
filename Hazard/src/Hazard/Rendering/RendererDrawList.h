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
		Ref<DescriptorSet> DescriptorSet;
		Ref<CubemapTexture> Cubemap;
		SkyboxConstants Constants;
	};

	//Draw list for single world context
	struct RendererDrawList
	{
		Ref<WorldRenderer> WorldRenderer;
		EnvironmentData Environment;
		DrawListStat Stats;
	};
}
