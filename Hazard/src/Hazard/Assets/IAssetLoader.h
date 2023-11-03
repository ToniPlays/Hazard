#pragma once

#include "Asset.h"
#include "Jobs.h"
#include "Ref.h"

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

		virtual Ref<JobGraph> Load(AssetMetadata& path) = 0;
		
		//TODO: Maybe a Revert option
		virtual Ref<JobGraph> Save(Ref<Asset>& asset) = 0;
		//Return asset data required to create 
		virtual Ref<JobGraph> DataFromSource(const std::filesystem::path& path) = 0;

		virtual Ref<JobGraph> Create(const std::filesystem::path& path) = 0;

		virtual Buffer ToBinary(Ref<Asset> asset) = 0;
	};
}
