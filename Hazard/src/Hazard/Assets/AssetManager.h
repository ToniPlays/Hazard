#pragma once

#include "Asset.h"
#include "AssetRegistry.h"
#include "AssetLoader.h"
#include "Hazard/Core/Core.h"
#include "UID.h"
#include "Profiling/PerformanceProfiler.h"
#include "Jobs.h"

#include "Hazard/Rendering/Mesh/Mesh.h"

#include <hzrpch.h>

namespace Hazard
{
	enum AssetManagerFlags : uint32_t
	{
		AssetManagerFlags_NoLoad = BIT(0),
		AssetManagerFlags_CanAsync = BIT(1)
	};


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
		static JobPromise SaveAssetAsync(Ref<Asset> asset);

		template<typename T>
		static TypedJobPromise<Ref<T>> GetAssetAsync(AssetHandle handle, uint32_t flags = 0)
		{
			static_assert(std::is_base_of<Asset, T>::value);

			AssetMetadata& meta = GetMetadata(handle);
			HZR_ASSERT(meta.Type != AssetType::Undefined, "AssetType cannot be Undefined for {0}", meta.Path.string());
			if (meta.LoadState != LoadState::Loaded)
			{
				meta.LoadState = LoadState::Loading;
				JobPromise promise = s_AssetLoader.LoadAsync(meta, flags | AssetManagerFlags_CanAsync);

				auto waitPromise = promise.TypedThen<Ref<T>>([handle](JobSystem* system, Job* job) -> size_t {

					AssetMetadata& meta = GetMetadata(handle);

					Job* dependency = system->GetJob(job->Dependency);
					Ref<T> asset = *dependency->Value<Ref<T>>();
					meta.LoadState = dependency->ReturnCode ? LoadState::Loaded : LoadState::None;

					if (meta.LoadState != LoadState::Loaded)
					{
						HZR_CORE_ERROR("{0} Failed to load with {1} ({2})", meta.Path.string(), dependency->ReturnCode, meta.Handle);
						return -1;
					}

					HZR_ASSERT(meta.Type != AssetType::Undefined, "AssetType cannot be Undefined for {0}", meta.Path.string());

					asset->SetHandle(meta.Handle);
					asset->SetFlags(AssetFlags::Valid);
					s_LoadedAssets[meta.Handle] = asset;

					*job->Value<Ref<T>>() = asset;
					return 0;
					});

				return waitPromise;
			}
			else
			{
				HZR_ASSERT(false, "This should not happen");
			}
			return TypedJobPromise<Ref<T>>();
		}
		template<typename T>
		static TypedJobPromise<Ref<T>> GetAssetAsync(const std::filesystem::path& path, uint32_t flags = 0)
		{
			return GetAssetAsync<T>(GetHandleFromFile(path), flags);
		}

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
			HZR_ASSERT(meta.Type != AssetType::Undefined, "AssetType cannot be Undefined for {0}", meta.Path.string());
			if (meta.LoadState != LoadState::Loaded && !(flags & AssetManagerFlags_NoLoad))
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
		static std::filesystem::path ToRelative(const std::filesystem::path& path);

	private:
		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		inline static AssetRegistry s_Registry;
		inline static AssetMetadata s_NullMetadata;
		inline static AssetLoader s_AssetLoader;
	};
}
