#pragma once

#include "Asset.h"
#include "Jobs.h"
#include "Ref.h"

namespace Hazard
{
	struct SaveAssetSettings;
	struct CreateAssetSettings;
	struct LoadAssetSettings;

	/// <summary>
	/// Interface for registering asset loaders for a given asset type
	/// </summary>
	class IAssetLoader 
	{
	public:
		IAssetLoader() = default;
		virtual ~IAssetLoader() = default;

		virtual Ref<JobGraph> Load(AssetMetadata& path, const LoadAssetSettings& settings) = 0;
		virtual Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) = 0;
		virtual Ref<JobGraph> Create(const CreateAssetSettings& settings) = 0;
	};
}
