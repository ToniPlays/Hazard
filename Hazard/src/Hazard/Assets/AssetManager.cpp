#include <hzrpch.h>
#include "AssetManager.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{
	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::s_LoadedAssets;

	void AssetManager::Init()
	{
		HZR_PROFILE_FUNCTION();
		s_Registry.Clear();
	}
	void AssetManager::Shutdown()
	{
		HZR_PROFILE_FUNCTION();
		s_LoadedAssets.clear();
	}
	/// <summary>
	/// Import asset, asset is not loaded until it is requested
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	AssetHandle AssetManager::ImportAsset(const AssetPackElement& asset, std::string key)
	{
		HZR_PROFILE_FUNCTION();

		if ((AssetType)asset.Type == AssetType::Undefined)
			return INVALID_ASSET_HANDLE;

		if (s_Registry.Contains(key) && !key.empty())
		{
			Ref<Asset> asset = GetAsset<Asset>(s_Registry.Get(key).Handle);
			HZR_CORE_WARN("Reimporting asset {} with {} references", key, asset->GetRefCount());
			s_Registry.Get(key).LoadState = LoadState::None;
			return s_Registry.Get(key).Handle;
		}

		AssetMetadata metadata = {};
		metadata.Type = (AssetType)asset.Type;
		metadata.Key = key;
		metadata.AssetPackFile = asset.AssetPack;
		metadata.Handle = asset.Handle;
		metadata.LoadState = LoadState::None;

		s_Registry[key] = metadata;

		HZR_CORE_INFO("Importing asset as {} ({})", key, asset.Handle);

		return asset.Handle;
	}
	void AssetManager::RemoveAsset(AssetHandle handle)
	{
		HZR_PROFILE_FUNCTION();
		if (handle == INVALID_ASSET_HANDLE) return;

		AssetMetadata& data = GetMetadata(handle);

		data.LoadState = LoadState::None;

		if (s_LoadedAssets.size() == 0) return;

		if (s_LoadedAssets.find(handle) != s_LoadedAssets.end())
			s_LoadedAssets.erase(handle);
	}
	AssetHandle AssetManager::GetHandleFromKey(const std::string& key)
	{
		HZR_PROFILE_FUNCTION();
		return s_Registry.Contains(key) ? s_Registry.Get(key).Handle : INVALID_ASSET_HANDLE;
	}
	Buffer AssetManager::GetAssetData(AssetHandle handle)
	{
		//Find correct file and read data to buffer
		AssetMetadata& metadata = GetMetadata(handle);

		CachedBuffer buffer = File::ReadBinaryFile(metadata.AssetPackFile);
		if (!buffer.GetData())
			return Buffer();

		AssetPack pack = AssetPack::Create(buffer, metadata.AssetPackFile);

		for (auto& element : pack.Elements)
		{
			if (element.Handle == handle)
				return Buffer::Copy(element.Data);
		}
		return Buffer();
	}
	bool AssetManager::IsLoaded(const AssetHandle& handle)
	{
		return s_LoadedAssets.find(handle) != s_LoadedAssets.end();
	}
	AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
	{
		HZR_PROFILE_FUNCTION();

		for (auto& [path, metadata] : s_Registry)
		{
			if (metadata.Handle == handle)
				return metadata;
		}
		return s_NullMetadata;
	}

	bool AssetManager::SaveAsset(Ref<Asset> asset)
	{
		HZR_ASSERT(asset, "Asset cannot be nullptr");

		Ref<JobGraph> graph = s_AssetLoader.Save(asset);
		if (graph)
		{
			graph->Execute();
			return true;
		}
		return false;
	}
}
