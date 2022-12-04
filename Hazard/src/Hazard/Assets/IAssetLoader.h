#pragma once
#include "Asset.h"
#include "Jobs.h"

namespace Hazard
{
	enum class LoadType
	{
		None = 0,
		Source,
		Cache,
		Failed
	};

	/// <summary>
	/// Interface for registering asset loaders for a given asset type
	/// </summary>
	class IAssetLoader 
	{
	public:
		IAssetLoader() = default;
		virtual ~IAssetLoader() = default;

		virtual LoadType Load(AssetMetadata& path, Ref<Asset>& asset) = 0;
		virtual JobPromise LoadAsync(AssetMetadata& path) = 0;
		//TODO: Maybe a Revert option
		virtual bool Save(Ref<Asset>& asset) = 0;
		virtual JobPromise SaveAsync(Ref<Asset>& asset) = 0;
	};
}
