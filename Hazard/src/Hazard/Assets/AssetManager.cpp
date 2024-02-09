#include <hzrpch.h>
#include "AssetManager.h"
#include "AssetPack.h"

#include "Hazard/Core/Application.h"
#include <Utility/StringUtil.h>

namespace Hazard
{
	void AssetManager::Init()
	{
		HZR_PROFILE_FUNCTION();
		s_Registry.Clear();
		s_IsInitialized = true;
	}

	void AssetManager::UnloadRequired()
	{
		if (s_AssetLoadCallback.Count() > 0)
		{
			s_AssetLoadCallback.Invoke();
			s_AssetLoadCallback.Clear();
		}

		for (auto& [handle, time] : s_UnloadAssetAfter)
		{
			if (time <= Time::s_Time)
			{
				s_AssetLoadCallback.Add([handle]() {
					Ref<Asset> asset = s_LoadedAssets[handle];
					if (!asset) return;
					if (asset->GetRefCount() == 2)
						asset->DecRefCount();

				});
			}
		}
	}

	void AssetManager::Shutdown()
	{
		HZR_PROFILE_FUNCTION();
		s_IsInitialized = false;

		std::scoped_lock lock(s_AssetMutex);
		for (auto& [handle, asset] : s_LoadedAssets)
			s_LoadedAssets.at(handle) = nullptr;

		s_LoadedAssets.clear();
	}

	AssetHandle AssetManager::Import(const std::filesystem::path& path)
	{
		HZR_PROFILE_FUNCTION();
		CachedBuffer buffer = File::ReadBinaryFile(path);
		AssetPack pack = {};
		pack.FromBuffer(buffer);

		HZR_CORE_ASSERT(!(pack.Flags & ASSET_PACK_HEADER && pack.Flags & ASSET_PACK_ELEMENT), "Invalid asset");

		if (pack.Flags & ASSET_PACK_HEADER)
			return ImportAssetPack(path, pack);
		else if (pack.Flags & ASSET_PACK_ELEMENT)
			return ImportAsset(path, pack);

		return INVALID_ASSET_HANDLE;
	}

	AssetHandle AssetManager::AssetHandleFromFile(const std::filesystem::path& file)
	{
		auto path = File::GetFileAbsolutePath(file);
		if (!s_Registry.Contains(path))
		{
			for (auto& [p, metadata] : s_Registry)
			{
				if (File::GetFileAbsolutePath(metadata.SourceFile) == path)
					return metadata.Handle;
			}
			return INVALID_ASSET_HANDLE;
		}
		return s_Registry.Get(path).Handle;
	}

	AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
	{
		HZR_PROFILE_FUNCTION();

		for (auto& [path, metadata] : s_Registry)
		{
			if (metadata.Handle == handle)
				return metadata;
		}
		return s_NullMetadata;
	}

	Ref<Asset> AssetManager::CreateAsset(AssetType type, const CreateAssetSettings& settings)
	{
		HZR_PROFILE_FUNCTION();
		JobPromise promise = CreateAssetAsync(type, settings);
		promise.Wait();

		return promise.GetResult<Ref<Asset>>();
	}

	JobPromise AssetManager::CreateAssetAsync(AssetType type, const CreateAssetSettings& settings)
	{
		Ref<JobGraph> graph = s_AssetLoader.Create(type, settings);
		if (!graph) return JobPromise();

		JobPromise promise = Application::Get().GetJobSystem().QueueGraph(graph);
		promise.Then([source = settings.SourcePath](JobGraph& graph) {
			Ref<Asset> asset = graph.GetResult<Ref<Asset>>();
			if (!asset) return;

			asset->m_Handle = UID();
			asset->m_SourceAssetPath = source;

			auto absolutePath = File::GetFileAbsolutePath(source);

			AssetMetadata metadata = {};
			metadata.AssetPackHandle = 0;
			metadata.Handle = asset->GetHandle();
			metadata.Type = asset->GetType();
			metadata.FilePath = "";
			metadata.SourceFile = source;
			metadata.LoadState = LoadState::Loaded;

			std::scoped_lock(s_AssetMutex);
			s_LoadedAssets[asset->GetHandle()] = asset;
			s_Registry[absolutePath] = metadata;
		});
		return promise;
	}

	JobPromise AssetManager::SaveAsset(Ref<Asset> asset, SaveAssetSettings settings)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(asset, "Asset cannot be nullptr");

		AssetMetadata& metadata = GetMetadata(asset->GetHandle());

		if (settings.Flags & ASSET_MANAGER_SAVE_AND_UPDATE && !settings.TargetPath.empty())
			metadata.FilePath = settings.TargetPath;

		if (!metadata.FilePath.empty())
			settings.TargetPath = metadata.FilePath;

		Ref<JobGraph> graph = s_AssetLoader.Save(asset, settings);

		if (!graph) return JobPromise();

		JobPromise promise = Application::Get().GetJobSystem().QueueGraph(graph);

		promise.Then([asset, settings](JobGraph& graph) {

			Buffer result = graph.GetResult<Buffer>();
			HZR_CORE_ASSERT(!settings.TargetPath.empty(), "Saving file to null path");

			//Save asset package
			AssetPack pack = {};
			pack.Flags = ASSET_PACK_ELEMENT;
			pack.Handle = asset->GetHandle();
			pack.Type = asset->GetType();
			pack.SourceFile = asset->GetSourceFilePath().string();

			std::scoped_lock lock(s_AssetMutex);

			//Copy save result to 
			if (settings.Flags & ASSET_MANAGER_COMBINE_ASSET)
			{
				pack.Flags |= ASSET_PACK_CONTAINS_DATA;
				pack.AssetData = result;
			}
			else
			{
				pack.Flags |= ASSET_PACK_REFERENCES_FILE;
				if (!pack.SourceFile.empty())
				{
					if (!File::WriteBinaryFile(pack.SourceFile, result.Data, result.Size))
						throw JobException("Could not save source file");
				}
			}
			auto buffer = pack.ToBuffer();
			if (!File::WriteBinaryFile(settings.TargetPath, buffer.GetData(), buffer.GetCursor()))
				throw JobException("Failed to save asset file");

			pack.AssetData.Release();
		});

		return promise;
	}

	void AssetManager::Unload(AssetHandle handle)
	{
		HZR_PROFILE_FUNCTION();
		if (handle == INVALID_ASSET_HANDLE) return;


		std::scoped_lock(s_AssetMutex);
		AssetMetadata& meta = GetMetadata(handle);
		meta.LoadState = LoadState::None;
		s_LoadedAssets.erase(handle);
	}
	JobPromise AssetManager::GetAssetAsync(AssetHandle handle)
	{
		HZR_PROFILE_FUNCTION();

		if (handle == INVALID_ASSET_HANDLE)
			return JobPromise();

		if (s_LoadedAssets[handle])
		{
			s_UnloadAssetAfter[handle] = Time::s_Time + ASSET_UNLOAD_TIME;

			Ref<JobGraph> graph = JobGraph::EmptyWithResult(s_LoadedAssets[handle]);
			return JobPromise(graph);
		}

		AssetMetadata& metadata = GetMetadata(handle);

		Ref<JobGraph> graph = s_AssetLoader.Load(metadata);
		if (!graph) return JobPromise();

		metadata.LoadState = LoadState::Loading;

		JobPromise promise = Application::Get().GetJobSystem().QueueGraph(graph);
		promise.Then([handle](JobGraph& graph) mutable {
			Ref<Asset> asset = graph.GetResult<Ref<Asset>>();
			if (!asset) return;

			AssetMetadata& metadata = GetMetadata(handle);
			asset->m_Handle = metadata.Handle;
			asset->m_SourceAssetPath = metadata.SourceFile;
			metadata.LoadState = LoadState::Loaded;

			std::scoped_lock lock(s_AssetMutex);
			s_UnloadAssetAfter[handle] = Time::s_Time + ASSET_UNLOAD_TIME;
			s_LoadedAssets[asset->GetHandle()] = asset;
		});

		return promise;
	}
	AssetHandle AssetManager::ImportAssetPack(const std::filesystem::path& path, const AssetPack& pack)
	{
		HZR_PROFILE_FUNCTION();
		HZR_CORE_ASSERT(false, "Asset pack import not supported");
		return AssetHandle();
	}
	AssetHandle AssetManager::ImportAsset(const std::filesystem::path& path, const AssetPack& pack)
	{
		auto absolutePath = File::GetFileAbsolutePath(path);
		AssetMetadata metadata = {};
		metadata.AssetPackHandle = 0;
		metadata.Handle = pack.Handle;
		metadata.Type = pack.Type;
		metadata.FilePath = absolutePath;
		metadata.SourceFile = pack.SourceFile;

		std::scoped_lock(s_AssetMutex);
		metadata.LoadState = s_LoadedAssets.contains(pack.Handle) ? LoadState::Loaded : LoadState::None;
		s_Registry[absolutePath] = metadata;

		HZR_CORE_WARN("Imported from {} with source {}", metadata.FilePath.string(), metadata.SourceFile.string());

		return pack.Handle;
	}
}
