#pragma once

#include "Asset.h"
#include "AssetRegistry.h"
#include "AssetLoader.h"
#include "Hazard/Core/Core.h"
#include "UID.h"
#include "Profiling/PerformanceProfiler.h"

namespace Hazard 
{
	class AssetManager {
	public:
		AssetManager() = default;
		~AssetManager() = default;

		static void Init();
		static void Shutdown();

		static std::unordered_map<std::filesystem::path, AssetMetadata>& GetMetadataRegistry() 
		{ 
			return s_Registry.GetRegistry(); 
		}
		
		template<typename T, typename... Args>
		static void RegisterLoader(AssetType type, Args... args) 
		{
			s_AssetLoader.m_Loaders[type] = CreateScope<T>(std::forward<Args>(args)...);
		}

		static AssetHandle ImportAsset(const std::filesystem::path& filePath, AssetMetadata metadata = AssetMetadata());
		static AssetHandle GetHandleFromFile(const std::filesystem::path& filePath);

		static void RemoveAsset(AssetHandle handle);
		static bool IsLoaded(const AssetHandle& handle);

		static AssetMetadata& GetMetadata(AssetHandle handle);

		static bool SaveAsset(Ref<Asset> asset);

		template<typename T>
		static Ref<T> GetAsset(const std::filesystem::path& path)
		{
			return GetAsset<T>(GetHandleFromFile(path));
		}

		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle) 
		{
			static_assert(std::is_base_of<Asset, T>::value);

			HZR_PROFILE_FUNCTION();
			HZR_TIMED_FUNCTION();

			AssetMetadata& meta = GetMetadata(handle);
			HZR_ASSERT(meta.Type != AssetType::Undefined, "AssetType cannot be Undefined for {0}", meta.Path.string());
			if (!meta.IsLoaded)
			{
				Ref<Asset> asset;
				LoadType loadType = s_AssetLoader.Load(meta, asset);
				meta.IsLoaded = loadType != LoadType::Failed;

				if (!meta.IsLoaded) 
					return nullptr;

				HZR_ASSERT(meta.Type != AssetType::Undefined, "AssetType cannot be Undefined for {0}", meta.Path.string());

				asset->SetHandle(meta.Handle);
				asset->SetFlags(AssetFlags::Valid);
				s_LoadedAssets[handle] = asset;
				return asset;
			}
			else
			{
				if (s_LoadedAssets.find(handle) == s_LoadedAssets.end()) 
					return nullptr;
				return s_LoadedAssets[handle].As<T>();
			}
		}
		
		static bool AddRuntimeAsset(const AssetMetadata& metadata, Ref<Asset> asset) 
		{
			HZR_PROFILE_FUNCTION();
			HZR_ASSERT(asset->m_Handle == metadata.Handle, "Stuff no match");

			asset->m_Type = metadata.Type;
			s_LoadedAssets[metadata.Handle] = asset;

			return true;
		}
		template<typename T>
		static Ref<T> GetRuntimeAsset(const AssetHandle& handle)
		{
			HZR_PROFILE_FUNCTION();
			if (handle == INVALID_ASSET_HANDLE) return nullptr;
			return s_LoadedAssets[handle].As<T>();
		}
		static void RemoveRuntimeAsset(const AssetHandle& handle)
		{
			HZR_PROFILE_FUNCTION();
			if (handle == INVALID_ASSET_HANDLE) return;
			s_LoadedAssets.erase(handle);
		}
		static std::filesystem::path ToRelative(const std::filesystem::path& path);

	private:
		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		inline static AssetRegistry s_Registry;
		inline static AssetMetadata s_NullMetadata;
		inline static AssetLoader s_AssetLoader;
	};
}
