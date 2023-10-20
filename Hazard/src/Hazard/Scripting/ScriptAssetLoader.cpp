
#include <hzrpch.h>
#include "ScriptAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "HScript.h"

namespace Hazard 
{
	static void LoadScriptAsset(Ref<Job> job, AssetHandle handle)
	{
		Buffer buffer = AssetManager::GetAssetData(handle);
		if (!buffer.Data)
			throw JobException("Failed to load script");

		std::string sourceFile = buffer.Read<std::string>();

		Ref<HScript> script = Ref<HScript>::Create(sourceFile, File::GetNameNoExt(sourceFile));
		script->IncRefCount();
		job->SetResult(&script, sizeof(Ref<HScript>));

		buffer.Release();
	}

	static void SaveScriptAsset(Ref<Job> job, Ref<HScript> asset, std::filesystem::path path)
	{
		if (!File::WriteFile(path, ""))
			throw JobException("Failed to create source for script: " + path.string());
		
		const AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());
		const AssetMetadata& packMetadata = AssetManager::GetMetadata(metadata.AssetPackHandle);

		std::string filePath = packMetadata.Handle != INVALID_ASSET_HANDLE ? packMetadata.Key : path.string();
        
		AssetHandle packHandle = metadata.AssetPackHandle != INVALID_ASSET_HANDLE ? metadata.AssetPackHandle : AssetHandle();

		asset->SetSourceFile(File::GetPathNoExt(path));
		Buffer buffer;
		buffer.Allocate(asset->GetSourceFile().string().length() + sizeof(uint64_t));
		buffer.Write(asset->GetSourceFile().string());

		AssetPackElement element = {};
		element.AssetPackHandle = packHandle;
		element.Handle = asset->GetHandle();
		element.Type = AssetType::Script;
		element.Data = buffer;
		element.AddressableName = File::GetName(path);

		AssetPack pack = {};
		pack.Handle = packHandle;
		pack.ElementCount = 1;
		pack.Elements = { element };

		CachedBuffer packBuffer = AssetPack::ToBuffer(pack);

		if (!File::WriteBinaryFile(filePath, packBuffer.GetData(), packBuffer.GetSize()))
			throw JobException(fmt::format("Saving asset pack failed: {0}", filePath));
        
		if (!File::WriteFile(File::GetPathNoExt(path), ""))
			throw JobException(fmt::format("Saving script source failed: {0}", File::GetPathNoExt(path).string()));
	}
	Ref<JobGraph> ScriptAssetLoader::Load(AssetMetadata& metadata)
	{
		Ref<HScript> script = Ref<HScript>::Create();
		script->m_Handle = AssetHandle();

		Ref<Job> loadJob = Ref<Job>::Create("Script load", LoadScriptAsset, metadata.Handle);

		Ref<JobGraph> graph = Ref<JobGraph>::Create("Script load", 1);
		Ref<GraphStage> createStage = graph->AddStage();
		createStage->QueueJobs({ loadJob });

		return graph;
	}
	Ref<JobGraph> ScriptAssetLoader::Save(Ref<Asset>& asset)
	{
		return nullptr;
	}
	Ref<JobGraph> ScriptAssetLoader::Create(const std::filesystem::path& path)
	{
		Ref<HScript> script = Ref<HScript>::Create();
		script->m_Handle = AssetHandle();

		Ref<Job> createJob = Ref<Job>::Create("Script create", SaveScriptAsset, script, path);

		Ref<JobGraph> graph = Ref<JobGraph>::Create("Script create", 1);
		Ref<GraphStage> createStage = graph->AddStage();
		createStage->QueueJobs({ createJob });

		return graph;
	}
}
