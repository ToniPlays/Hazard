#pragma once

#include "Asset.h"
#include "AssetRegistry.h"
#include "AssetLoader.h"
#include "AssetPack.h"
#include "UID.h"
#include "Profiling/PerformanceProfiler.h"
#include "Jobs.h"

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

		static std::unordered_map<std::filesystem::path, AssetMetadata>& GetMetadataRegistry() { return s_Registry.GetRegistry(); }

		template<typename T, typename... Args>
		static void RegisterLoader(AssetType type, Args&&... args)
		{
			s_AssetLoader.m_Loaders[type] = CreateScope<T>(std::forward<Args>(args)...);
		}

		static AssetHandle ImportAssetPack(const AssetPack& pack, const std::filesystem::path& path);
		static AssetHandle ImportAsset(const AssetPackElement& element);
		static AssetHandle GetHandleFromKey(const std::string& key);
		static Buffer GetAssetData(AssetHandle handle);

		static AssetPack OpenAssetPack(AssetHandle handle);

		static void RemoveAsset(AssetHandle handle);
		static bool IsLoaded(const AssetHandle& handle);

		static AssetMetadata& GetMetadata(AssetHandle handle);
		static bool SaveAsset(Ref<Asset> asset);
		static std::vector<AssetMetadata> GetAllAssetMetadata(AssetType type);

		template<typename T>
		static Ref<T> GetAsset(const std::string& key)
		{
			return GetAsset<T>(GetHandleFromKey(key));
		}

		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			static_assert(std::is_base_of<Asset, T>::value);

			HZR_PROFILE_FUNCTION();
			HZR_TIMED_FUNCTION();

			if (handle == INVALID_ASSET_HANDLE)
				return nullptr;

			AssetMetadata& metadata = GetMetadata(handle);
			if (metadata.Type == AssetType::Undefined)
				return nullptr;

			if (metadata.LoadState == LoadState::None)
			{
				metadata.LoadState = LoadState::Loading;

				//Load asset async and wait
				Ref<JobGraph> graph = s_AssetLoader.Load(metadata);
				if (!graph)
					return nullptr;

				Ref<Asset> asset = graph->Execute()->GetResult<Ref<Asset>>();
				if (asset)
				{
					metadata.LoadState = LoadState::Loaded;
					asset->m_Type = metadata.Type;
					asset->m_Handle = metadata.Handle;
				}
				else
					metadata.LoadState = LoadState::None;

				std::scoped_lock lock(s_Mutex);

				s_LoadedAssets[metadata.Handle] = asset;
				return asset.As<T>();
			}
			return s_LoadedAssets[metadata.Handle].As<T>();
		}

		static JobPromise GetAssetAsync(const std::string& key)
		{
			return GetAssetAsync(GetHandleFromKey(key));
		}

		static JobPromise GetAssetAsync(AssetHandle handle)
		{
			HZR_PROFILE_FUNCTION();
			HZR_TIMED_FUNCTION();

			if (handle == INVALID_ASSET_HANDLE)
				return JobPromise();

			AssetMetadata& metadata = GetMetadata(handle);

			if (metadata.LoadState == LoadState::None)
			{
				metadata.LoadState = LoadState::Loading;
				s_LoadedAssets[handle] = nullptr;

				//Load asset async and wait
				Ref<JobGraph> graph = s_AssetLoader.Load(metadata);
				if (graph)
				{
					Ref<Job> job = Ref<Job>::Create("Add loaded asset", AddLoadedAssetJob);
					graph->AddStage()->QueueJobs({ job });
				}
				return Application::Get().GetJobSystem().QueueGraph(graph);
			}

			return JobPromise();
		}

		static JobPromise CreateFromSource(const std::filesystem::path& file)
		{
			Ref<JobGraph> graph = s_AssetLoader.FromSourceFile(file);
			if (!graph) return JobPromise();

			return Application::Get().GetJobSystem().QueueGraph(graph);
		}
		static AssetHandle CreateMemoryOnly(AssetType type, Ref<Asset> asset);
		static AssetHandle CreateNewAsset(AssetType type, const std::filesystem::path& path);
		static Buffer AssetToBinary(Ref<Asset> asset);

	private:
		static void AddLoadedAssetJob(JobInfo& info)
		{
			auto assets = info.PreviousStage->GetJobResults<Ref<Asset>>();
			if (!assets.size() == 0) return;

			std::scoped_lock lock(s_Mutex);
			for (auto& asset : assets)
			{
				auto& metadata = GetMetadata(asset->GetHandle());
				metadata.LoadState = LoadState::Loaded;
				s_LoadedAssets[metadata.Handle] = asset;
			}
		}

	private:

		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		inline static AssetRegistry s_Registry;
		inline static AssetMetadata s_NullMetadata;
		inline static AssetLoader s_AssetLoader;

		inline static bool s_IsInitialized = false;
		inline static std::mutex s_Mutex;
	};
}
