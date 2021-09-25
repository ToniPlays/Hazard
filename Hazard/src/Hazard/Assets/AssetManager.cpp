#pragma once
#include <hzrpch.h>
#include "AssetManager.h"

namespace Hazard 
{

    std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::s_LoadedAssets;
    AssetRegistry AssetManager::s_Registry;
    AssetMetadata AssetManager::s_NullMetadata;
    AssetLoader AssetManager::s_AssetLoader;

    void AssetManager::Init()
    {
        s_Registry.Clear();
    }
    void AssetManager::Shutdown()
    {
        s_LoadedAssets.clear();
    }
    AssetHandle AssetManager::ImportAsset(const std::string& filePath)
    {
        //TODO: proper relative path
        std::filesystem::path path = std::filesystem::relative(filePath, std::filesystem::absolute(APPLICATION_PERSISTENT_PATH));

        if (s_Registry.Contains(path)) {
            return s_Registry.Get(path).Handle;
        }

        AssetType type = Utils::AssetTypeFromExtension(File::GetFileExtension(path.string()));
        if (type == AssetType::None)
            return INVALID_ASSET_HANDLE;

        AssetMetadata data;
        data.Handle = AssetHandle();
        data.Path = path;
        data.Type = type;
        data.IsLoaded = false;

        s_Registry[data.Path] = data;
        HZR_CORE_INFO("[AssetManager]: Importing asset {0}", data.Path.string());

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
        return std::filesystem::relative(path, APPLICATION_PERSISTENT_PATH);
        
    }
}