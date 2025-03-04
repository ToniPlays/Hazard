#include <hzrpch.h>
#include "AssetManager.h"
#include "AssetPack.h"

#include "Hazard/Core/Application.h"
#include <Utility/StringUtil.h>
#include <Filesystem/Directory.h>

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
		s_AssetLoadCallback.Invoke();
		s_AssetLoadCallback.Clear();

		for (auto& [handle, time] : s_UnloadAssetAfter)
		{
			if (time > Time::s_Time) continue;

			s_AssetLoadCallback.Add([handle]() {
				Unload(handle);
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
		AssetPack pack = OpenAssetPack(path);

		if (pack.Flags & ASSET_PACK_HEADER)
			return ImportAssetPack(path, pack);
		else if (pack.Flags & ASSET_PACK_ELEMENT)
			return ImportAsset(path, pack);

		return INVALID_ASSET_HANDLE;
	}

	AssetPack AssetManager::OpenAssetPack(const std::filesystem::path& path)
	{
		Ref<CachedBuffer> buffer = File::ReadBinaryFile(path);
		AssetPack pack = {};
		pack.FromBuffer(buffer);

		HZR_CORE_ASSERT(!(pack.Flags & ASSET_PACK_HEADER && pack.Flags & ASSET_PACK_ELEMENT), "Invalid asset");
		return pack;
	}

	AssetHandle AssetManager::AssetHandleFromFile(const std::filesystem::path& file)
	{
		HZR_TIMED_FUNCTION();
		HZR_PROFILE_FUNCTION();
		auto path = File::GetFileAbsolutePath(file);

		if (s_Registry.Contains(path))
			return s_Registry.Get(path).Handle;

		for (auto& [p, metadata] : s_Registry)
		{
			auto absSource = File::GetFileAbsolutePath(metadata.SourceFile);

			if (absSource.compare(path) == 0 && !absSource.empty())
				return metadata.Handle;
		}

		return INVALID_ASSET_HANDLE;
	}

	bool AssetManager::IsAssetLoaded(AssetHandle handle)
	{
		HZR_TIMED_FUNCTION();
		return GetMetadata(handle).LoadState == LoadState::Loaded;
	}

	AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
	{
		HZR_TIMED_FUNCTION();
		HZR_PROFILE_FUNCTION();

		for (auto& [path, metadata] : s_Registry)
		{
			if (metadata.Handle == handle)
				return metadata;
		}
		return s_NullMetadata;
	}

	void AssetManager::Unload(AssetHandle handle)
	{
		HZR_PROFILE_FUNCTION();
		if (handle == INVALID_ASSET_HANDLE) return;

		std::scoped_lock lock(s_AssetMutex);

		if (s_LoadedAssets.find(handle) == s_LoadedAssets.end()) return;
		if (!s_LoadedAssets[handle])
		{
			s_LoadedAssets.erase(handle);
			return;
		}

		if (s_LoadedAssets[handle]->GetRefCount() >= 2) return;

		HZR_TIMED_FUNCTION();

		AssetMetadata& meta = GetMetadata(handle);
		meta.LoadState = LoadState::None;

		s_UnloadAssetAfter.erase(handle);
		s_LoadedAssets.erase(handle);

		HZR_CORE_INFO("Unloaded {}", meta.Handle);
	}

	Ref<JobGraph> AssetManager::GetLoadGraph(AssetMetadata& metadata, LoadAssetSettings settings)
	{
		HZR_TIMED_FUNCTION();
		Ref<JobGraph> graph = s_AssetLoader.Load(metadata, settings);
		if (!graph) return nullptr;


		graph->AddOnFinished([graph = graph.Raw(), handle = metadata.Handle]() mutable {
			Ref<Asset> asset = graph->GetResults<Ref<Asset>>()[0];
			if (!asset) return;

			AssetMetadata& metadata = GetMetadata(handle);
			asset->m_Handle = metadata.Handle;
			asset->m_SourceAssetPath = metadata.SourceFile;
			metadata.LoadState = LoadState::Loaded;
			metadata.IterationID = UID();

			std::scoped_lock lock(s_AssetMutex);
			s_LoadedAssets[asset->GetHandle()] = asset;
			s_UnloadAssetAfter[handle] = Time::s_Time + ASSET_UNLOAD_TIME;

			HZR_CORE_INFO("Loaded {}", metadata.Handle);

		});

		graph->AddOnFailed([](const JobException& e) {
			HZR_CORE_ERROR(e.what());
			});

		return graph;
	}

	Ref<JobGraph> AssetManager::GetSaveGraph(Ref<Asset> asset, SaveAssetSettings settings)
	{
		HZR_ASSERT(asset, "Asset cannot be nullptr");

		AssetMetadata& metadata = GetMetadata(asset->GetHandle());
		if (settings.Flags & ASSET_MANAGER_SAVE_AND_UPDATE && !settings.TargetPath.empty() && metadata.Handle != INVALID_ASSET_HANDLE)
			metadata.FilePath = settings.TargetPath;

		if (!metadata.FilePath.empty() && settings.TargetPath.empty())
			settings.TargetPath = metadata.FilePath;

		Ref<JobGraph> graph = s_AssetLoader.Save(asset, settings);
		if (!graph) return nullptr;

		graph->AddOnFinished([graph = graph.Raw(), asset, settings]() {

			Ref<CachedBuffer> result = graph->GetResults<Ref<CachedBuffer>>()[0];
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
			}

			auto buffer = pack.ToBuffer();
			if (!File::WriteBinaryFile(settings.TargetPath, buffer->GetData(), buffer->GetCursor()))
				throw JobException(fmt::format("Could not save asset file: {}", settings.TargetPath.string()));

			AssetMetadata& metadata = GetMetadata(asset->GetHandle());
			if (!metadata.IsValid())
				ImportAsset(settings.TargetPath, pack);

			HZR_CORE_INFO("Saving asset: Target: {} ({})", settings.TargetPath.string(), Utils::AssetTypeToString(asset->GetType()));
			});

		return graph;
	}

	Ref<JobGraph> AssetManager::GetCreateGraph(CreateAssetSettings settings)
	{
		return s_AssetLoader.Create(settings);
	}

	AssetHandle AssetManager::ImportAssetPack(const std::filesystem::path& path, const AssetPack& pack)
	{
		HZR_TIMED_FUNCTION();
		HZR_PROFILE_FUNCTION();
		HZR_CORE_ASSERT(false, "Asset pack import not yet supported");

		return AssetHandle();
	}

	AssetHandle AssetManager::ImportAsset(const std::filesystem::path& path, const AssetPack& pack)
	{
		HZR_TIMED_FUNCTION();
		auto absolutePath = File::GetFileAbsolutePath(path);

		std::scoped_lock lock(s_AssetMutex);

		AssetMetadata metadata = {
			.AssetPackHandle = 0,
			.Handle = pack.Handle,
			.Type = pack.Type,
			.LoadState = s_LoadedAssets.contains(pack.Handle) ? LoadState::Loaded : LoadState::None,
			.PackFlags = pack.Flags,
			.FilePath = absolutePath,
			.SourceFile = pack.SourceFile
		};

		HZR_CORE_ASSERT(metadata.Type != AssetType::Undefined, "Asset cannot be undefined");

		s_Registry[absolutePath] = metadata;

		return pack.Handle;
	}
}
