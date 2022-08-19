#include <hzrpch.h>
#include "AssetManager.h"

namespace Hazard
{
	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::s_LoadedAssets;

	void AssetManager::Init()
	{
		s_Registry.Clear();
	}
	void AssetManager::Shutdown()
	{
		s_LoadedAssets.clear();
	}
	/// <summary>
	/// Import asset, asset is not loaded until it is requested
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	AssetHandle AssetManager::ImportAsset(const std::filesystem::path& filePath, AssetMetadata metadata)
	{
		if (filePath == "") return INVALID_ASSET_HANDLE;

		std::filesystem::path path = File::GetFileAbsolutePath(filePath); //std::filesystem::relative(filePath, std::filesystem::absolute(APPLICATION_PERSISTENT_PATH));

		if (s_Registry.Contains(path))
			return s_Registry.Get(path).Handle;

		AssetType type = AssetType::Undefined;

		if (!File::IsDirectory(filePath)) {
			std::string extension = File::GetFileExtension(path.string());
			type = Utils::AssetTypeFromExtension(extension);
		}
		else type = AssetType::Folder;

		if (type == AssetType::Undefined)
			return INVALID_ASSET_HANDLE;

		//Create Asset metadata, don't load until requested
 		if (!metadata.IsValid()) {
			metadata.Handle = AssetHandle();
			metadata.Path = path;
			metadata.Type = type;
		}
		s_Registry[metadata.Path] = metadata;

		return metadata.Handle;
	}
	void AssetManager::RemoveAsset(AssetHandle handle)
	{
		bool found = false;
		for (const auto& item : s_Registry) {
			if (item.second.Handle == handle) {
				found = true;
				break;
			}
		}
		if (!found) return;

		AssetMetadata& data = GetMetadata(handle);
		s_Registry.Remove(data.Path);

		if (s_LoadedAssets.find(handle) != s_LoadedAssets.end())
			s_LoadedAssets.erase(handle);
	}
	AssetHandle AssetManager::GetHandleFromFile(const std::string& filePath)
	{
		std::filesystem::path path = filePath;

		if (s_Registry.Contains(path)) {
			return s_Registry.Get(path).Handle;
		}
		return INVALID_ASSET_HANDLE;
	}
	bool AssetManager::IsLoaded(const AssetHandle& handle)
	{
		return s_LoadedAssets.find(handle) != s_LoadedAssets.end();
	}
	AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
	{
		for (auto& [path, metadata] : s_Registry) {
			if (metadata.Handle == handle)
				return metadata;
		}
		return s_NullMetadata;
	}
	std::filesystem::path AssetManager::ToRelative(const std::filesystem::path& path)
	{
		return path;
		return std::filesystem::relative(path, APPLICATION_PERSISTENT_PATH);
	}
}
