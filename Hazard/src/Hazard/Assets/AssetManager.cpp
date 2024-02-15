#include <hzrpch.h>
#include "AssetManager.h"
#include "AssetPack.h"

#include "Hazard/Core/Application.h"
#include <Utility/StringUtil.h>
#include <Directory.h>

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
			if (time > Time::s_Time) continue;

			s_AssetLoadCallback.Add([handle]() {

				Ref<Asset> asset = s_LoadedAssets[handle];
				if (!asset) return;

				if (asset->GetRefCount() == 2)
					asset->DecRefCount();

			});
		}
	}

	void AssetManager::Shutdown()
	{
		HZR_PROFILE_FUNCTION();
		s_IsInitialized = false;

		std::scoped_lock lock(s_AssetMutex);
		s_LoadedAssets.clear();
	}

	AssetHandle AssetManager::Import(const std::filesystem::path& path)
	{
		HZR_PROFILE_FUNCTION();
		Ref<CachedBuffer> buffer = File::ReadBinaryFile(path);
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

		if (s_Registry.Contains(path))
			return s_Registry.Get(path).Handle;

		for (auto& [p, metadata] : s_Registry)
		{
			auto abs = File::GetFileAbsolutePath(metadata.SourceFile);
			if (abs.compare(path) == 0 && !abs.empty())
				return metadata.Handle;
		}

		return INVALID_ASSET_HANDLE;
	}

	bool AssetManager::IsAssetLoaded(AssetHandle handle)
	{
		return GetMetadata(handle).LoadState == LoadState::Loaded;
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
		Ref<JobGraph> graph = GetCreateGraph(type, settings);
		if (!graph) return JobPromise();

		return Application::Get().GetJobSystem().QueueGraph(graph);
	}

	JobPromise AssetManager::SaveAsset(Ref<Asset> asset, SaveAssetSettings settings)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(asset, "Asset cannot be nullptr");

		AssetMetadata& metadata = GetMetadata(asset->GetHandle());
		if (settings.Flags & ASSET_MANAGER_SAVE_AND_UPDATE && !settings.TargetPath.empty() && metadata.Handle != INVALID_ASSET_HANDLE)
			metadata.FilePath = settings.TargetPath;

		if (!metadata.FilePath.empty())
			settings.TargetPath = metadata.FilePath;

		HZR_CORE_ASSERT(!settings.TargetPath.empty(), "Saving file to null path");

		Ref<JobGraph> graph = GetSaveGraph(asset, settings);
		if (!graph) return JobPromise();

		return Application::Get().GetJobSystem().QueueGraph(graph);
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

	JobPromise AssetManager::Reload(AssetHandle handle)
	{
		std::scoped_lock(s_AssetMutex);

		AssetMetadata& metadata = AssetManager::GetMetadata(handle);
		if (metadata.LoadState == LoadState::None) return JobPromise();

		Ref<JobGraph> graph = s_AssetLoader.Load(metadata);
		if (!graph) return JobPromise();

		Ref<Asset> oldAsset = s_LoadedAssets[handle];

		graph->AddOnCompleted([handle, oldAsset](JobGraph& graph) {
			Ref<Asset> asset = graph.GetResult<Ref<Asset>>();
			if (!asset) return;

			asset->m_Handle = oldAsset->GetHandle();
			asset->m_Flags = oldAsset->GetFlags();
			asset->m_SourceAssetPath = oldAsset->GetSourceFilePath();

			s_LoadedAssets[handle] = asset;
		});

		return Application::Get().GetJobSystem().QueueGraph(graph);
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
		if (!metadata.IsValid()) return JobPromise();

		Ref<JobGraph> graph = GetLoadGraph(metadata);

		if (!graph || metadata.LoadState != LoadState::None)
			return JobPromise();

		metadata.LoadState = LoadState::Loading;
		return Application::Get().GetJobSystem().QueueGraph(graph);
	}

	Ref<JobGraph> AssetManager::GetLoadGraph(AssetMetadata& metadata)
	{
		Ref<JobGraph> graph = s_AssetLoader.Load(metadata);
		if (!graph) return nullptr;

		graph->AddOnCompleted([handle = metadata.Handle](JobGraph& graph) mutable {
			Ref<Asset> asset = graph.GetResult<Ref<Asset>>();
			if (!asset) return;

			AssetMetadata& metadata = GetMetadata(handle);
			asset->m_Handle = metadata.Handle;
			asset->m_SourceAssetPath = metadata.SourceFile;
			metadata.LoadState = LoadState::Loaded;

			std::scoped_lock lock(s_AssetMutex);
			s_LoadedAssets[asset->GetHandle()] = asset;
			s_UnloadAssetAfter[handle] = Time::s_Time + ASSET_UNLOAD_TIME;
		});
		return graph;
	}

	Ref<JobGraph> AssetManager::GetSaveGraph(Ref<Asset> asset, SaveAssetSettings settings)
	{
		Ref<JobGraph> graph = s_AssetLoader.Save(asset, settings);
		if (!graph) return nullptr;

		graph->AddOnCompleted([asset, settings](JobGraph& graph) {

			Ref<CachedBuffer> result = graph.GetResult<Ref<CachedBuffer>>();
			if (!result)
				throw JobException("Did not receive asset data");

			//Save asset package
			AssetPack pack = {};
			pack.Flags = ASSET_PACK_ELEMENT;
			pack.Handle = asset->GetHandle();
			pack.Type = asset->GetType();
			pack.SourceFile = asset->GetSourceFilePath().string();

			auto directoryPath = File::GetDirectoryOf(settings.TargetPath);

			if (!Directory::Exists(directoryPath))
				Directory::Create(directoryPath);

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
					if (!File::WriteBinaryFile(pack.SourceFile, result->GetData(), result->GetSize()))
						throw JobException(fmt::format("Could not save source file: {}", pack.SourceFile));
				}
			}

			auto buffer = pack.ToBuffer();
			if (!File::WriteBinaryFile(settings.TargetPath, buffer->GetData(), buffer->GetSize()))
				throw JobException(fmt::format("Could not save asset file: {}", settings.TargetPath.string()));

			AssetMetadata& metadata = GetMetadata(asset->GetHandle());
			if (!metadata.IsValid())
				ImportAsset(settings.TargetPath, pack);
		});

		return graph;
	}

	Ref<JobGraph> AssetManager::GetCreateGraph(AssetType type, CreateAssetSettings settings)
	{
		Ref<JobGraph> graph = s_AssetLoader.Create(type, settings);
		if (!graph) return nullptr;

		graph->AddOnCompleted([source = settings.SourcePath](JobGraph& graph) {
			Ref<Asset> asset = graph.GetResult<Ref<Asset>>();
			if (!asset) return;

			asset->m_Handle = UID();
			asset->m_SourceAssetPath = source;

			auto absolutePath = File::GetFileAbsolutePath(source);

			AssetMetadata metadata = {
				.AssetPackHandle = 0,
				.Handle = asset->GetHandle(),
				.Type = asset->GetType(),
				.LoadState = LoadState::Loaded,
				.FilePath = "",
				.SourceFile = source,
			};

			std::scoped_lock(s_AssetMutex);
			s_LoadedAssets[asset->GetHandle()] = asset;
		});
		return graph;
	}

	AssetHandle AssetManager::ImportAssetPack(const std::filesystem::path& path, const AssetPack& pack)
	{
		HZR_PROFILE_FUNCTION();
		HZR_CORE_ASSERT(false, "Asset pack import not yet supported");

		return AssetHandle();
	}

	AssetHandle AssetManager::ImportAsset(const std::filesystem::path& path, const AssetPack& pack)
	{
		auto absolutePath = File::GetFileAbsolutePath(path);

		std::cout << absolutePath.string() << std::endl;

		std::scoped_lock lock(s_AssetMutex);

		AssetMetadata metadata = {
			.AssetPackHandle = 0,
			.Handle = pack.Handle,
			.Type = pack.Type,
			.LoadState = s_LoadedAssets.contains(pack.Handle) ? LoadState::Loaded : LoadState::None,
			.FilePath = absolutePath,
			.SourceFile = pack.SourceFile
		};

		s_Registry[absolutePath] = metadata;

		return pack.Handle;
	}
}
