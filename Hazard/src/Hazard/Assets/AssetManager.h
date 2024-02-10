#pragma once

#include "Asset.h"
#include "AssetRegistry.h"
#include "AssetLoader.h"
#include "UID.h"
#include "Profiling/PerformanceProfiler.h"
#include "Jobs.h"

#include "Hazard/Core/Application.h"

#include <hzrpch.h>

#define ASSET_UNLOAD_TIME 5

namespace Hazard
{
	class AssetPack;

	enum AssetManagerFlags : uint32_t
	{
		ASSET_MANAGER_SAVE_AND_UPDATE = BIT(0),
		ASSET_MANAGER_COMBINE_ASSET = BIT(1),
	};

	struct SaveAssetSettings
	{
		std::filesystem::path TargetPath;
		uint32_t Flags = ASSET_MANAGER_SAVE_AND_UPDATE;
	};
	struct CreateAssetSettings
	{
		std::filesystem::path SourcePath;
		void* Settings = nullptr;
	};

	class AssetManager
	{
	public:
		AssetManager() = default;
		~AssetManager() = default;

		static void Init();
		static void UnloadRequired();
		static void Shutdown();

		static std::unordered_map<std::filesystem::path, AssetMetadata>& GetMetadataRegistry() { return s_Registry.GetRegistry(); }

		template<typename T, typename... Args>
		static void RegisterLoader(AssetType type, Args&&... args)
		{
			s_AssetLoader.m_Loaders[type] = CreateScope<T>(std::forward<Args>(args)...);
		}

		static AssetHandle Import(const std::filesystem::path& path);
		static AssetHandle AssetHandleFromFile(const std::filesystem::path& file);

		static AssetMetadata& GetMetadata(AssetHandle handle);
		static Ref<Asset> CreateAsset(AssetType type, const CreateAssetSettings& settings);
		static JobPromise CreateAssetAsync(AssetType type, const CreateAssetSettings& settings);
		static JobPromise SaveAsset(Ref<Asset> asset, SaveAssetSettings settings = SaveAssetSettings());

		static void Unload(AssetHandle handle);
		static JobPromise Reload(AssetHandle handle);

		template<typename T>
		static Ref<T> GetAsset(const std::filesystem::path& path)
		{
			return GetAsset<T>(AssetHandleFromFile(path));
		}
		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			HZR_PROFILE_FUNCTION();
			if (handle == INVALID_ASSET_HANDLE)
				return nullptr;

			if (s_LoadedAssets[handle] != nullptr)
			{
				s_UnloadAssetAfter[handle] = Time::s_Time + ASSET_UNLOAD_TIME;
				return s_LoadedAssets[handle].As<T>();
			}

			AssetMetadata& metadata = GetMetadata(handle);
			if (metadata.Type == AssetType::Undefined) 
				return nullptr;

			JobPromise promise = GetAssetAsync(handle);
			promise.Wait();
			s_UnloadAssetAfter[handle] = Time::s_Time + ASSET_UNLOAD_TIME;
			return promise.GetResult<Ref<T>>();
		}

		static JobPromise GetAssetAsync(AssetHandle handle);

	private:
		static AssetHandle ImportAssetPack(const std::filesystem::path& path, const AssetPack& pack);
		static AssetHandle ImportAsset(const std::filesystem::path& path, const AssetPack& pack);

	private:
		inline static std::mutex s_AssetMutex;
		inline static std::mutex s_UnloadMutex;
		inline static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		inline static AssetRegistry s_Registry;
		inline static AssetMetadata s_NullMetadata;
		inline static AssetLoader s_AssetLoader;
		inline static Callback<void()> s_AssetLoadCallback;
		inline static std::unordered_map<AssetHandle, double> s_UnloadAssetAfter;

		inline static bool s_IsInitialized = false;
	};
}
