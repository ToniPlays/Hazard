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

		static std::unordered_map<std::filesystem::path, AssetMetadata>& GetMetadataRegistry()
		{
			return s_Registry.GetRegistry();
		}

		template<typename T, typename... Args>
		static void RegisterLoader(AssetType type, Args... args)
		{
			s_AssetLoader.m_Loaders[type] = CreateScope<T>(std::forward<Args>(args)...);
		}
		static AssetHandle ImportAssetPack(const AssetPack& pack, const std::filesystem::path& path);
		static AssetHandle ImportAsset(const AssetPackElement& pack, std::string name = "");
		static AssetHandle GetHandleFromKey(const std::string& key);
		static Buffer GetAssetData(AssetHandle handle);

		static AssetPack OpenAssetPack(AssetHandle handle);

		static void RemoveAsset(AssetHandle handle);
		static bool IsLoaded(const AssetHandle& handle);

		static AssetMetadata& GetMetadata(AssetHandle handle);

		static bool SaveAsset(Ref<Asset> asset);

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
				if (!graph) return nullptr;
				graph->Execute();

				Ref<Asset> asset = graph->GetResult<Ref<Asset>>();
				if (asset)
				{
					asset->m_Type = metadata.Type;
					asset->m_Handle = metadata.Handle;
				}

				std::scoped_lock lock(s_Mutex);

				s_LoadedAssets[metadata.Handle] = asset;
				return s_LoadedAssets[metadata.Handle].As<T>();
			}
			return s_LoadedAssets[metadata.Handle].As<T>();
		}

		template<typename T>
		static JobPromise<Ref<T>> GetAssetAsync(const std::string& key)
		{
			return GetAssetAsync<T>(GetHandleFromKey(key));
		}

		template<typename T>
		static JobPromise<Ref<T>> GetAssetAsync(AssetHandle handle)
		{
			static_assert(std::is_base_of<Asset, T>::value);

			HZR_PROFILE_FUNCTION();
			HZR_TIMED_FUNCTION();

			if (handle == INVALID_ASSET_HANDLE)
				return JobPromise<Ref<T>>();

			AssetMetadata& metadata = GetMetadata(handle);

			if (metadata.LoadState == LoadState::None)
			{
				metadata.LoadState = LoadState::Loading;
				s_LoadedAssets[handle] = nullptr;

				//Load asset async and wait
				Ref<JobGraph> graph = s_AssetLoader.Load(metadata);
				if (graph)
				{
					Ref<Job> job = Ref<Job>::Create(AddLoadedAssetJop, handle);
					graph->AddStage()->QueueJobs({ job });
				}
				JobPromise<Ref<Asset>> promise = Application::Get().GetJobSystem().QueueGraph<Ref<Asset>>(graph);
				return promise;
			}
			return JobPromise<Ref<T>>(s_LoadedAssets[handle]);
		}

		static AssetHandle CreateMemoryOnly(AssetType type, Ref<Asset> asset)
		{
			AssetMetadata metadata = {};
			metadata.Handle = AssetHandle();
			metadata.Type = type;
			metadata.MemoryOnly = true;
			metadata.LoadState = LoadState::Loaded;

			asset->m_Flags |= (uint32_t)AssetFlags::MemoryOnly;
			asset->m_Handle = metadata.Handle;

			s_Registry[std::to_string(asset->GetHandle())] = metadata;

			s_LoadedAssets[metadata.Handle] = asset;
			return metadata.Handle;
		}

	private:
		static void AddLoadedAssetJop(Ref<Job> job, AssetHandle handle)
		{
			Ref<Asset> asset = job->GetInput<Ref<Asset>>();
			if (!asset) return;

			job->GetStage()->SetResult(asset);
			auto& metadata = GetMetadata(handle);

			metadata.LoadState = LoadState::Loaded;

			std::scoped_lock lock(s_Mutex);
			s_LoadedAssets[handle] = asset;
		}

	private:

		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		inline static AssetRegistry s_Registry;
		inline static AssetMetadata s_NullMetadata;
		inline static AssetLoader s_AssetLoader;

		inline static std::mutex s_Mutex;
	};
}
