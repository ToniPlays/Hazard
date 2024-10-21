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
		ASSET_MANAGER_NO_DEPENENCY_LOADING = BIT(2),
	};

	struct LoadAssetSettings
	{
		uint32_t Flags = 0;
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
		static bool IsAssetLoaded(AssetHandle handle);

		static AssetMetadata& GetMetadata(AssetHandle handle);
        template<typename T>
		static Ref<T> CreateAsset(const CreateAssetSettings& settings)
        {
            HZR_PROFILE_FUNCTION();
            Promise<Ref<T>> promise = CreateAssetAsync<T>(AssetType::Image, settings);
            promise.Wait();
            return nullptr;
            
        }
        template<typename T>
        static Promise<Ref<T>> CreateAssetAsync(AssetType type, const CreateAssetSettings& settings)
        {
            Ref<JobGraph> graph = GetCreateGraph(type, settings);
            if (!graph) return Promise<Ref<T>>();

            return Application::Get().GetJobSystem().Submit<Ref<T>>(graph);
        }
        template<typename T>
        static Promise<Ref<T>> SaveAsset(Ref<Asset> asset, SaveAssetSettings settings = SaveAssetSettings())
        {
            HZR_PROFILE_FUNCTION();

            Ref<JobGraph> graph = GetSaveGraph(asset, settings);
            if (!graph) return Promise<Ref<T>>();

            return Application::Get().GetJobSystem().Submit<Ref<T>>(graph);
        }

		static void Unload(AssetHandle handle);
        template<typename T>
		static Promise<T> Reload(AssetHandle handle)
        {
            std::scoped_lock lock(s_AssetMutex);

            AssetMetadata& metadata = AssetManager::GetMetadata(handle);
            if (metadata.LoadState == LoadState::None) return Promise<T>();

            Ref<JobGraph> graph = s_AssetLoader.Load(metadata, LoadAssetSettings());
            if (!graph) return Promise<T>();

            Ref<Asset> oldAsset = s_LoadedAssets[handle];

            /*graph->AddOnCompleted([handle, oldAsset](JobGraph& graph) {
                Ref<Asset> asset = graph.GetResult<Ref<Asset>>();
                if (!asset) return;

                asset->m_Handle = oldAsset->GetHandle();
                asset->m_Flags = oldAsset->GetFlags();
                asset->m_SourceAssetPath = oldAsset->GetSourceFilePath();

                s_LoadedAssets[handle] = asset;
            });
             */

            return Application::Get().GetJobSystem().Submit<Ref<T>>(graph);
        }

		template<typename T>
		static Ref<T> GetAsset(const std::filesystem::path& path, LoadAssetSettings settings = LoadAssetSettings())
		{
			return GetAsset<T>(AssetHandleFromFile(path), settings);
		}

		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle, LoadAssetSettings settings = LoadAssetSettings())
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

			Promise<Ref<T>> promise = GetAssetAsync<T>(handle, settings);
			promise.Wait();

			s_UnloadAssetAfter[handle] = Time::s_Time + ASSET_UNLOAD_TIME;
            return nullptr; //promise.Result();
		}
        template<typename T>
		static Promise<Ref<T>> GetAssetAsync(AssetHandle handle, LoadAssetSettings settings = LoadAssetSettings())
        {
            HZR_PROFILE_FUNCTION();

            if (handle == INVALID_ASSET_HANDLE)
                return Promise<Ref<T>>();

            if (s_LoadedAssets[handle])
            {
                s_UnloadAssetAfter[handle] = Time::s_Time + ASSET_UNLOAD_TIME;
                Ref<JobGraph> graph = JobGraph::EmptyWithResult(s_LoadedAssets[handle]);
                return Promise<Ref<T>>();
            }

            AssetMetadata& metadata = GetMetadata(handle);
            if (!metadata.IsValid()) return Promise<Ref<T>>();

            Ref<JobGraph> graph = GetLoadGraph(metadata, settings);

            if (!graph || metadata.LoadState != LoadState::None)
                return Promise<Ref<T>>();

            metadata.LoadState = LoadState::Loading;
            return Application::Get().GetJobSystem().Submit<Ref<T>>(graph);
        }

		static Ref<JobGraph> GetLoadGraph(AssetMetadata& metadata, LoadAssetSettings settings = LoadAssetSettings());
		static Ref<JobGraph> GetSaveGraph(Ref<Asset> asset, SaveAssetSettings settings = SaveAssetSettings());
		static Ref<JobGraph> GetCreateGraph(AssetType type, CreateAssetSettings settings = CreateAssetSettings());

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
