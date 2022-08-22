#pragma once

#include "Asset.h"
#include "AssetRegistry.h"
#include "AssetLoader.h"
#include "Hazard/Core/Core.h"
#include "UID.h"

namespace Hazard 
{
	class AssetManager {
	public:
		AssetManager() = default;
		~AssetManager() = default;

		static void Init();
		static void Shutdown();

		static std::unordered_map<std::filesystem::path, AssetMetadata>& GetMetadataRegistry() { 
			return s_Registry.GetRegistry(); 
		}
		
		template<typename T, typename... Args>
		static void RegisterLoader(AssetType type, Args... args) 
		{
			s_AssetLoader.m_Loaders[type] = CreateScope<T>(std::forward<Args>(args)...);
		}

		static AssetHandle ImportAsset(const std::filesystem::path& filePath, AssetMetadata metadata = AssetMetadata());
		static AssetHandle GetHandleFromFile(const std::string& filePath);

		static void RemoveAsset(AssetHandle handle);
		static bool IsLoaded(const AssetHandle& handle);

		static AssetMetadata& GetMetadata(AssetHandle handle);

		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle) 
		{
			AssetMetadata& meta = GetMetadata(handle);

			if (!meta.IsLoaded)
			{
				Ref<Asset> asset;
				meta.IsLoaded = s_AssetLoader.Load(meta, asset);
				if (!meta.IsLoaded) {
					return nullptr;
				}
				asset->SetHandle(meta.Handle);
				asset->SetFlags(AssetFlags::Valid);
				s_LoadedAssets[handle] = asset;
				return asset;
			}
			else
				return s_LoadedAssets[handle].As<T>();
		}
		
		static bool AddRuntimeAsset(const AssetMetadata& metadata, Ref<Asset> asset) 
		{
			assert(asset->m_Handle == metadata.Handle, "Handles don't match, stuff will break");
			asset->m_Type = metadata.Type;
			s_LoadedAssets[metadata.Handle] = asset;
			return true;
		}
		template<typename T>
		static Ref<T> GetRuntimeAsset(const AssetHandle& handle)
		{
			return s_LoadedAssets[handle].As<T>();
		}
		static std::filesystem::path ToRelative(const std::filesystem::path& path);

	private:
		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		inline static AssetRegistry s_Registry;
		inline static AssetMetadata s_NullMetadata;
		inline static AssetLoader s_AssetLoader;
	};
}
