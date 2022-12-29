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
	AssetHandle AssetManager::ImportAsset(const std::filesystem::path& filePath, AssetMetadata metadata)
	{
		HZR_PROFILE_FUNCTION();
		if (filePath == "") return INVALID_ASSET_HANDLE;

		std::filesystem::path path = File::GetFileAbsolutePath(filePath); 

		if (s_Registry.Contains(path))
			return s_Registry.Get(path).Handle;

		AssetType type = AssetType::Undefined;

		if (!File::IsDirectory(filePath)) 
		{
			std::string extension = File::GetFileExtension(path.string());
			type = Utils::AssetTypeFromExtension(extension);
		}
		else type = AssetType::Folder;

		if (type == AssetType::Undefined)
			return INVALID_ASSET_HANDLE;

		//Create Asset metadata, don't load until requested
 		if (!metadata.IsValid()) 
		{
			metadata.Handle = AssetHandle();
			metadata.Path = path;
			metadata.Type = type;
		}
		s_Registry[metadata.Path] = metadata;

		return metadata.Handle;
	}
	void AssetManager::RemoveAsset(AssetHandle handle)
	{
		HZR_PROFILE_FUNCTION();
		if (handle == INVALID_ASSET_HANDLE) return;

		AssetMetadata& data = GetMetadata(handle);

		data.LoadState = LoadState::None;
		s_Registry.Remove(data.Path);

		if (s_LoadedAssets.size() == 0) return;

		if (s_LoadedAssets.find(handle) != s_LoadedAssets.end())
			s_LoadedAssets.erase(handle);
	}
	AssetHandle AssetManager::GetHandleFromFile(const std::filesystem::path& filePath)
	{
		HZR_PROFILE_FUNCTION();
		auto path = File::GetFileAbsolutePath(filePath);
		return s_Registry.Contains(path) ? s_Registry.Get(path).Handle : INVALID_ASSET_HANDLE;
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
		return s_AssetLoader.Save(asset);
	}
	JobPromise AssetManager::SaveAssetAsync(Ref<Asset> asset)
	{
		HZR_ASSERT(asset, "Asset cannot be nullptr");
		Ref<JobGraph> graph = s_AssetLoader.SaveAsync(asset);
		return Application::Get().GetJobSystem().SubmitGraph(graph);
	}

	std::filesystem::path AssetManager::ToRelative(const std::filesystem::path& path)
	{
		return path;
	}
}
