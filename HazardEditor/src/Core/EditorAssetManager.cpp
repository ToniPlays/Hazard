
#include "EditorAssetManager.h"
#include "Hazard/Assets/AssetManager.h"
#include "File.h"
#include "Utility/YamlUtils.h"
#include "Hazard.h"
#include <Hazard/RenderContext/ShaderAsset.h>
#include <src/Core/GUIManager.h>
#include <src/GUI/Overlays/ProgressOverlay.h>

#include <FileCache.h>

using namespace Hazard;

void EditorAssetManager::Init()
{
	ImportEngineAssets();
}
void EditorAssetManager::LoadEditorAssets()
{
	struct EditorAsset
	{
		std::string Key;
		std::string Path;
	};

	std::vector<EditorAsset> texturesToLoad = {
		{ "Default", "Library/textureBG.png.hpack"},
		{ "Folder",  "Library/folder.png.hpack"},
		{ "World",   "Library/world.png.hpack"},
		{ "Script",  "Library/csharp.png.hpack"},
		{ "Camera",  "Library/camera.png.hpack"},
		{ "DirectionalLight", "Library/directionalLight.png.hpack"}
	};
	std::vector<EditorAsset> meshesToLoad = {
		{ "Cube", "Library/cube.obj.hpack"     },
		{ "Sphere", "Library/sphere.obj.hpack" }
	};

	Timer timer;
	for (auto& texture : texturesToLoad)
	{
		//Get asset pack handle
		AssetHandle packHandle = AssetManager::GetHandleFromKey(texture.Path);
		AssetPack pack = AssetManager::OpenAssetPack(packHandle);
		//s_Icons[texture.Key] = pack.Elements[0].Handle;
		pack.Free();
	}

	for (auto& mesh : meshesToLoad)
	{
		//Get asset pack handle
		AssetHandle packHandle = AssetManager::GetHandleFromKey(mesh.Path);
		AssetPack pack = AssetManager::OpenAssetPack(packHandle);
		//s_Icons[mesh.Key] = pack.Elements[0].Handle;
		pack.Free();
	}
}

AssetHandle EditorAssetManager::GetIconHandle(const std::string& name)
{
	if (s_Icons.find(name) != s_Icons.end())
		return s_Icons[name];
	return s_Icons["Default"];
}
AssetHandle EditorAssetManager::GetDefaultMesh(const std::string& name)
{
	if (s_DefaultMesh.find(name) != s_DefaultMesh.end())
		return s_DefaultMesh[name];
	return INVALID_ASSET_HANDLE;
}

void EditorAssetManager::GenerateAndSavePack(JobInfo& info, std::filesystem::path& path)
{
	FileCache cache("Library");

	JobPromise promise = AssetManager::CreateFromSource(path);
	promise.Wait();
	if (!promise.Succeeded())
		throw JobException(fmt::format("Cannot load source from file: {}", path.string()));

	std::vector<AssetPackElement> elements;

	for (auto& asset : promise.GetResults<Ref<Asset>>())
	{
		AssetPackElement& element = elements.emplace_back();
		element.AddressableName = File::GetName(path);
		element.Handle = asset->GetHandle();
		element.Type = asset->GetType();
		element.Data = AssetManager::AssetToBinary(asset);
	}

	AssetPack pack = {};
	pack.Handle = AssetHandle();
	pack.ElementCount = elements.size();
	pack.Elements = elements;

	for (auto& e : pack.Elements)
		e.AssetPackHandle = pack.Handle;

	CachedBuffer buffer = AssetPack::ToBuffer(pack);

	if(!cache.WriteFile("Shaders" / std::filesystem::path(File::GetName(path) + ".hpack"), buffer.GetData(), buffer.GetSize()))
	   throw JobException("Failed to save asset pack");

	pack.Free();
}

void EditorAssetManager::ImportEngineAssets()
{
	using namespace Hazard;
	HZR_INFO("Importing engine assets");

	Timer timer;
	FileCache cache("Library");

	std::vector<Ref<Job>> jobs;

	for (auto& file : Directory::GetAllInDirectory("res", true))
	{
		if (File::IsDirectory(file)) continue;

		auto packPath = File::GetName(file) + ".hpack";
		if (cache.HasFile(packPath)) continue;

		//TODO change
		auto extension = File::GetFileExtension(file);
		if (extension == ".glslh") continue;
		if (extension == ".ttf") continue;
		if (extension == ".mtl") continue;
		if (extension == ".txt") continue;
		if (extension == ".ico") continue;
		if (extension == ".cs") continue;

		Ref<Job> job = Ref<Job>::Create(fmt::format("Generate pack: {0}", file.string()), GenerateAndSavePack, file);
		jobs.push_back(job);
	}

	if (jobs.size() > 0)
	{
		JobSystem& system = Application::Get().GetJobSystem();

		Ref<JobGraph> loadingGraph = Ref<JobGraph>::Create("Engine asset Load", 1, JOB_FLAGS_SILENT_FAILURE);
		loadingGraph->GetStage(0)->QueueJobs(jobs);

		JobPromise promise = system.QueueGraph(loadingGraph);
		promise.Wait();
		
		HZR_CORE_ASSERT(promise.Succeeded(), "Loading engine assets has failed");
	}

	auto files = Directory::GetAllInDirectory(cache.GetCachePath(), true);
	for (auto& file : files)
	{
		if (File::IsDirectory(file)) continue;

		auto packPath = File::GetName(file);

		CachedBuffer buffer = File::ReadBinaryFile(file);
		AssetPack pack = AssetPack::Create(buffer);
		AssetManager::ImportAssetPack(pack, file);
		pack.Free();
	}
	HZR_INFO("Engine assets imported in {}ms", timer.ElapsedMillis());
}