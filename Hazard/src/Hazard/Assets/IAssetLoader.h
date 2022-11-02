#pragma once
#include "Asset.h"

namespace Hazard
{
	enum class LoadType
	{
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
		//TODO: Maybe a Revert option
		virtual bool Save(Ref<Asset>& asset) = 0;
	};
}
