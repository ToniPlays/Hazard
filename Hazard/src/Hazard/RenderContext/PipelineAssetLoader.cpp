
#include <hzrpch.h>
#include "Hazard/Core/Application.h"
#include "PipelineAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "Core/Rendering/RenderPass.h"

#include "Core/GraphicsContext.h"
#include "ShaderAsset.h"

namespace Hazard
{
	using namespace HazardRenderer;

	Ref<JobGraph> PipelineAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();
		return nullptr;
	}

	Ref<JobGraph> PipelineAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		HZR_PROFILE_FUNCTION();
		return nullptr;
	}

	Ref<JobGraph> PipelineAssetLoader::Create(const CreateAssetSettings& settings)
	{
		return nullptr;
	}
}
