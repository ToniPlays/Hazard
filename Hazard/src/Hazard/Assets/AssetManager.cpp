
#include <hzrpch.h>
#include "AssetManager.h"

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
	AssetHandle AssetManager::ImportAsset(const std::string& filePath)
	{
		if (filePath == "") return INVALID_ASSET_HANDLE;
		//TODO: Proper relative path
		std::filesystem::path path = filePath; //std::filesystem::relative(filePath, std::filesystem::absolute(APPLICATION_PERSISTENT_PATH));

		if (s_Registry.Contains(path)) {
			return s_Registry.Get(path).Handle;
		}
		std::string extension = File::GetFileExtension(path.string());
		AssetType type = Utils::AssetTypeFromExtension(extension);

		if (type == AssetType::Undefined)
			return INVALID_ASSET_HANDLE;

		//Create Asset metadata, don't load until requested
		AssetMetadata data;
		data.Handle = AssetHandle();
		data.Path = path;
		data.Type = type;

		s_Registry[data.Path] = data;

		return data.Handle;
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
		if (!found)
			return;

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
