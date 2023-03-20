#pragma once

#include "Asset.h"
#include "AssetRegistry.h"
#include "AssetLoader.h"
#include "AssetPack.h"
#include "UID.h"
#include "Profiling/PerformanceProfiler.h"
#include "JobPromise.h"

#include "Hazard/Core/Application.h"

#include <hzrpch.h>

namespace Hazard
{
	enum AssetManagerFlags : uint32_t
	{
		AssetManagerFlags_MustBeLoaded = BIT(0),
		AssetManagerFlags_ForceReload = BIT(1),
		AssetManagerFlags_CanAsync = BIT(2),
		AssetManagerFlags_ForceSource = BIT(3)
	};


	class AssetManager
	{
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

		static AssetHandle ImportAsset(const AssetPack& pack);
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

			AssetMetadata& metadata = GetMetadata(handle);

			if (metadata.LoadState == LoadState::None)
			{
				Ref<Asset> asset = nullptr;
				//Load asset async and wait
				JobPromise<bool> promise = s_AssetLoader.Load(metadata, asset);
				promise.Wait();

				s_LoadedAssets[metadata.Handle] = asset;
				return asset.As<T>();
			}
			return s_LoadedAssets[metadata.Handle].As<T>();
		}

		template<typename T>
		static JobPromise<Ref<T>> GetAssetAsync(const std::filesystem::path& path, uint32_t flags = 0)
		{
			return JobPromise<Ref<T>>();
		}

		template<typename T>
		static JobPromise<Ref<T>> GetAssetAsync(AssetHandle handle, uint32_t flags = 0)
		{
			static_assert(std::is_base_of<Asset, T>::value);

			HZR_PROFILE_FUNCTION();
			HZR_TIMED_FUNCTION();

			return JobPromise<Ref<T>>();
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
	private:

		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		inline static AssetRegistry s_Registry;
		inline static AssetMetadata s_NullMetadata;
		inline static AssetLoader s_AssetLoader;
	};
}
