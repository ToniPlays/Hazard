#pragma once
#if 0
#include "Asset.h"
#include "AssetRegistry.h"
#include "AssetLoader.h"
#include "Hazard/Core/Core.h"
#include "Hazard/Core/UUID.h"

namespace Hazard 
{
	class AssetManager {
	public:
		AssetManager() = default;
		~AssetManager() = default;

		static void Init();
		static void Shutdown();
		static void RemoveUnreferencedResources();

		static std::unordered_map<std::filesystem::path, AssetMetadata> GetMetadataRegistry() { 
			return s_Registry.GetRegistry(); 
		}
		static std::unordered_map<AssetHandle, Ref<RuntimeResource>> GetRuntimeResources() 
		{
			return s_RuntimeResources;
		}
		
		template<typename T>
		static void RegisterLoader(AssetType type) 
		{
			s_AssetLoader.RegisterLoader<T>(type);
		}

		static AssetHandle ImportAsset(const std::string& filePath);
		static AssetHandle GetHandleFromFile(const std::string& filePath);

		static void RemoveAsset(AssetHandle handle);
		static bool IsLoaded(const AssetHandle& handle);

		static AssetMetadata& GetMetadata(AssetHandle handle);

		static void AddRuntimeResource(Ref<RuntimeResource> resource) 
		{
			resource->m_Handle = UUID();
			resource->DecRefCount();
			s_RuntimeResources[resource->GetHandle()] = resource;
		}
		static void RemoveRuntimeResource(AssetHandle handle)
		{
			s_RuntimeResources.erase(handle);
		}
		template<typename T>
		static Ref<T> GetRuntimeResource(AssetHandle handle) {
			if (s_RuntimeResources.find(handle) == s_RuntimeResources.end()) 
				return nullptr;

			Ref<T> resource = s_RuntimeResources[handle].As<T>();
			return resource;
		}

		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle) 
		{
			AssetMetadata& meta = GetMetadata(handle);
			Ref<Asset> asset = nullptr;

			if (!meta.IsLoaded)
			{
				meta.IsLoaded = s_AssetLoader.Load(meta, asset);
				if (!meta.IsLoaded) {
					return nullptr;
				}
				asset->SetHandle(meta.Handle);
				asset->SetFlags(AssetFlags::Valid);
				s_LoadedAssets[handle] = asset;
			}
			else
				asset = s_LoadedAssets[handle];

			return asset.As<T>();
		}

		static std::filesystem::path ToRelative(const std::filesystem::path& path);

	private:
		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		static std::unordered_map<AssetHandle, Ref<RuntimeResource>> s_RuntimeResources;
		inline static AssetRegistry s_Registry;
		inline static AssetMetadata s_NullMetadata;
		inline static AssetLoader s_AssetLoader;
	};
}
#endif