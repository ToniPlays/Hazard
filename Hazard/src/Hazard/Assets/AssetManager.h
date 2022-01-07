#pragma once

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
		
		template<typename T>
		static void RegisterLoader(AssetType type) 
		{
			s_AssetLoader.RegisterLoader<T>(type);
		}

		void CreateRuntimeAsset(Ref<Asset> asset) 
		{

			
		}

		static AssetHandle ImportAsset(const std::string& filePath);
		static AssetHandle GetHandleFromFile(const std::string& filePath);

		static void RemoveAsset(AssetHandle handle);
		static bool IsLoaded(const AssetHandle& handle);

		static AssetMetadata& GetMetadata(AssetHandle handle);

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
		inline static AssetRegistry s_Registry;
		inline static AssetMetadata s_NullMetadata;
		inline static AssetLoader s_AssetLoader;
	};
}