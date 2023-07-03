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
	AssetHandle AssetManager::ImportAssetPack(const AssetPack& pack, const std::filesystem::path& path)
	{
		HZR_PROFILE_FUNCTION();

		if (pack.ElementCount == 0)
			return INVALID_ASSET_HANDLE;

		auto filePath = path.lexically_normal();

		if (s_Registry.Contains(filePath))
		{
			AssetMetadata& metadata = s_Registry.Get(filePath);
			metadata.LoadState = LoadState::None;

			for (auto& element : pack.Elements)
				ImportAsset(element);

			return s_Registry.Get(filePath).Handle;
		}

		AssetMetadata metadata = {};
		metadata.AssetPackHandle = INVALID_ASSET_HANDLE;
		metadata.Type = AssetType::Last;
		metadata.Key = filePath.string();
		metadata.Handle = pack.Handle;
		metadata.LoadState = LoadState::None;

		s_Registry[filePath] = metadata;

		for (auto& element : pack.Elements)
			ImportAsset(element);

		return pack.Handle;
	}
	/// <summary>
	/// Import asset, asset is not loaded until it is requested
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	AssetHandle AssetManager::ImportAsset(const AssetPackElement& element)
	{
		HZR_PROFILE_FUNCTION();
		HZR_CORE_WARN("Importing asset: {0} ({1})", element.AddressableName, element.Handle);

		if ((AssetType)element.Type == AssetType::Undefined)
			return INVALID_ASSET_HANDLE;

		if (s_Registry.Contains(element.AddressableName))
		{
			Ref<Asset> asset = GetAsset<Asset>(GetHandleFromKey(element.AddressableName));
			AssetHandle handle = asset->GetHandle();
			asset->DecRefCount();
			s_Registry.Get(element.AddressableName).LoadState = LoadState::None;

			s_LoadedAssets[asset->GetHandle()] = GetAsset<Asset>(handle);

			return s_Registry.Get(element.AddressableName).Handle;
		}

		AssetMetadata metadata = {};
		metadata.AssetPackHandle = element.AssetPackHandle;
		metadata.Handle = element.Handle;
		metadata.Type = (AssetType)element.Type;
		metadata.LoadState = LoadState::None;
		metadata.Key = element.AddressableName;

		s_Registry[element.AddressableName] = metadata;
		return element.Handle;
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
		AssetMetadata& packMetadata = GetMetadata(metadata.AssetPackHandle);
		if (packMetadata.Handle == INVALID_ASSET_HANDLE)
			return Buffer();

		CachedBuffer buffer = File::ReadBinaryFile(packMetadata.Key);
		if (!buffer.GetData())
			return Buffer();

		AssetPack pack = AssetPack::Create(buffer);

		for (auto& element : pack.Elements)
		{
			if (element.Handle == handle)
				return Buffer::Copy(element.Data);
		}
		return Buffer();
	}

	AssetPack AssetManager::OpenAssetPack(AssetHandle handle)
	{
		AssetMetadata& packMetadata = GetMetadata(handle);
		CachedBuffer buffer = File::ReadBinaryFile(packMetadata.Key);

		if (!buffer.GetData())
			return AssetPack();

		return AssetPack::Create(buffer);
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
		if (!graph) return false;

		graph->Execute();
		return true;
	}
}
