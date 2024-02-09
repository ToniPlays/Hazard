#pragma once

#include "Asset.h"
#include "Jobs.h"
#include "Ref.h"

namespace Hazard
{
	struct SaveAssetSettings;
	struct CreateAssetSettings;

	/// <summary>
	/// Interface for registering asset loaders for a given asset type
	/// </summary>
	class IAssetLoader 
	{
	public:
		IAssetLoader() = default;
		virtual ~IAssetLoader() = default;

		virtual Ref<JobGraph> Load(AssetMetadata& path) = 0;
		
		//TODO: Maybe a Revert option
		virtual Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) = 0;

		virtual Ref<JobGraph> Create(const CreateAssetSettings& settings) = 0;
	};
}
