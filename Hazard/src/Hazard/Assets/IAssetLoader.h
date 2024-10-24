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

		virtual LoadType Load(AssetMetadata& path, Ref<Asset>& asset, uint32_t flags = 0) = 0;
		virtual TypedJobPromise<Ref<Asset>> LoadAsync(AssetMetadata& path, uint32_t flags = 0) = 0;
		//TODO: Maybe a Revert option
		virtual bool Save(Ref<Asset>& asset) = 0;
		virtual JobPromise SaveAsync(Ref<Asset>& asset) = 0;
	};
}
