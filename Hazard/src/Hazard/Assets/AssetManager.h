#pragma once

#include "Asset.h"
#include "AssetRegistry.h"
#include "AssetLoader.h"
#include "Hazard/Core/Core.h"
#include "UID.h"
#include "Profiling/PerformanceProfiler.h"
#include "Jobs.h"

#include "Hazard/Rendering/Mesh/Mesh.h"
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

		static AssetHandle ImportAsset(const std::filesystem::path& filePath, AssetMetadata metadata = AssetMetadata());
		static AssetHandle GetHandleFromFile(const std::filesystem::path& filePath);

		static void RemoveAsset(AssetHandle handle);
		static bool IsLoaded(const AssetHandle& handle);

		static AssetMetadata& GetMetadata(AssetHandle handle);

		static bool SaveAsset(Ref<Asset> asset);
		//static JobPromise SaveAssetAsync(Ref<Asset> asset);

		/*template<typename T>
		static JobPromise GetAssetAsync(AssetHandle handle, uint32_t flags = 0)
		{
			static_assert(std::is_base_of<Asset, T>::value);

			AssetMetadata& meta = GetMetadata(handle);
			HZR_ASSERT(meta.Type != AssetType::Undefined, "AssetType cannot be Undefined for {0}", meta.Path.string());

			if ((meta.LoadState == LoadState::None && !(flags & AssetManagerFlags_MustBeLoaded)) || flags & AssetManagerFlags_ForceReload)
			{
				meta.LoadState = LoadState::Loading;
				Ref<JobGraph> jobGraph = s_AssetLoader.LoadAsync(meta, flags);
				jobGraph->OnFinished([handle](JobGraph& graph) -> size_t {

					AssetMetadata& meta = GetMetadata(handle);

					Ref<T> asset = *graph.Result<Ref<T>>();
					meta.LoadState = asset ? LoadState::Loaded : LoadState::None;

					if (meta.LoadState != LoadState::Loaded)
					{
						HZR_CORE_ERROR("{0} Failed to load with {1} ({2})", meta.Path.string(), 0, meta.Handle);
						return -1;
					}

					HZR_ASSERT(meta.Type != AssetType::Undefined, "AssetType cannot be Undefined for {0}", meta.Path.string());

					asset->SetHandle(meta.Handle);
					asset->SetFlags(AssetFlags::Valid);
					s_LoadedAssets[meta.Handle] = asset;
					return 0;
					});
				return Application::Get().GetJobSystem().SubmitGraph(jobGraph);
			}

			Ref<T> asset;
			if (s_LoadedAssets.find(handle) == s_LoadedAssets.end())
				asset = nullptr;
			else
				asset = s_LoadedAssets[handle].As<T>();

			Ref<JobGraph> graph = Ref<JobGraph>::Create();
			graph->CreateBuffer<Ref<T>>();
			*graph->Result<Ref<T>>() = asset;

			return Application::Get().GetJobSystem().SubmitGraph(graph);
		}
		template<typename T>
		static JobPromise GetAssetAsync(const std::filesystem::path& path, uint32_t flags = 0)
		{
			return GetAssetAsync<T>(GetHandleFromFile(path), flags);
		}*/

		template<typename T>
		static Ref<T> GetAsset(const std::filesystem::path& path, uint32_t flags = 0)
		{
			return GetAsset<T>(GetHandleFromFile(path), flags);
		}

		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle, uint32_t flags = 0)
		{
			static_assert(std::is_base_of<Asset, T>::value);

			HZR_PROFILE_FUNCTION();
			HZR_TIMED_FUNCTION();

			AssetMetadata& meta = GetMetadata(handle);

			if ((meta.LoadState == LoadState::None && !(flags & AssetManagerFlags_MustBeLoaded)) || flags & AssetManagerFlags_ForceReload)
			{
				Ref<Asset> asset;
				LoadType loadType = s_AssetLoader.Load(meta, asset, flags);
				meta.LoadState = loadType != LoadType::Failed ? LoadState::Loaded : LoadState::None;

				if (meta.LoadState != LoadState::Loaded)
				{
					HZR_CORE_ERROR("{0} Failed to load with {1} ({2})", meta.Path.string(), (size_t)meta.LoadState, meta.Handle);
					return nullptr;
				}

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
		static AssetHandle NewAsset(Ref<Asset> asset)
		{
			if (asset->m_Handle == INVALID_ASSET_HANDLE)
				asset->m_Handle = UID();
			return asset->m_Handle;
		}
		static std::filesystem::path ToRelative(const std::filesystem::path& path);

	private:
		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		inline static AssetRegistry s_Registry;
		inline static AssetMetadata s_NullMetadata;
		inline static AssetLoader s_AssetLoader;
	};
}
