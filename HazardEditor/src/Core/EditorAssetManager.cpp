
#include "EditorAssetManager.h"
#include "Hazard/Assets/AssetManager.h"
#include "File.h"
#include "Utility/YamlUtils.h"
#include "Hazard.h"
#include <Hazard/RenderContext/ShaderAsset.h>
#include <src/Core/GUIManager.h>
#include <src/GUI/Overlays/ProgressOverlay.h>

#include "EditorAssetPackBuilder.h"

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
		s_Icons[texture.Key] = pack.Elements[0].Handle;
		pack.Free();
	}

	for (auto& mesh : meshesToLoad)
	{
		//Get asset pack handle
		AssetHandle packHandle = AssetManager::GetHandleFromKey(mesh.Path);
		AssetPack pack = AssetManager::OpenAssetPack(packHandle);
		s_Icons[mesh.Key] = pack.Elements[0].Handle;
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

	auto packPath = File::GetName(path) + ".hpack";
	CachedBuffer buffer = EditorAssetManager::GenerateEngineAssetPack(path);

	if (buffer.GetSize() == 0)
		throw JobException(fmt::format("Asset pack generation failed: {}", path.string()));

	if (!File::WriteBinaryFile(cache.GetCachePath() / packPath, buffer.GetData(), buffer.GetSize()))
		throw JobException(fmt::format("Asset pack saving failed: {}", path.string()));
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

	Ref<JobGraph> loadingGraph = Ref<JobGraph>::Create("EngineLoad", 1, JOB_FLAGS_SILENT_FAILURE);
	loadingGraph->GetStage(0)->QueueJobs(jobs);


	JobSystem& system = Application::Get().GetJobSystem();
	if (jobs.size() > 0)
	{
		JobPromise promise = system.QueueGraph(loadingGraph);
		promise.Wait();
		HZR_CORE_ASSERT(promise.Succeeded(), "Loading engine assets has failed");
	}

	auto files = Directory::GetAllInDirectory(cache.GetCachePath(), true);
	for (auto& file : files)
	{
		auto packPath = File::GetName(file);

		CachedBuffer buffer = File::ReadBinaryFile(file);
		AssetPack pack = AssetPack::Create(buffer);
		AssetManager::ImportAssetPack(pack, file);
		pack.Free();
	}
	HZR_INFO("Engine assets imported in {}ms", timer.ElapsedMillis());
}

CachedBuffer EditorAssetManager::GenerateEngineAssetPack(const std::filesystem::path& path)
{
	AssetType type = Hazard::Utils::AssetTypeFromExtension(File::GetFileExtension(path));

	if (type == AssetType::Undefined)
		return CachedBuffer();

	std::vector<AssetPackElement> elements;

	switch (type)
	{
		case AssetType::Shader:
		{
			Ref<JobGraph> graph = EditorAssetPackBuilder::CreatePackElement(path, RenderAPI::Vulkan);

			if (!graph) break;

			Buffer result = graph->Execute()->GetResult();
			AssetPackElement element = result.Read<AssetPackElement>();
			element.Handle = AssetHandle();
			elements.push_back(element);

			break;
		}
		case AssetType::Image:
		{
			Image2DCreateInfo info = {};
			info.DebugName = File::GetName(path);
			info.GenerateMips = false;
			info.Usage = ImageUsage::Texture;

			UI::ImageImportSettings settings = {};
			settings.GenerateMips = false;
			settings.FlipOnLoad = true;

			Buffer result = EditorAssetPackBuilder::CreatePackElement(path, info, settings)->Execute()->GetResult();
			AssetPackElement element = result.Read<AssetPackElement>();
			element.Handle = AssetHandle();
			elements.push_back(element);

			break;
		}
		case AssetType::Mesh:
		{
			MeshCreateInfo info = {};
			info.DebugName = File::GetName(path);

			UI::MeshImportSettings settings = {};

			Buffer result = EditorAssetPackBuilder::CreatePackElement(path, info, settings)->Execute()->GetResult();
			AssetPackElement element = result.Read<AssetPackElement>();
			element.Handle = AssetHandle();
			elements.push_back(element);
		}
	}
	if (elements.size() == 0)
		return CachedBuffer();

	AssetPack pack = EditorAssetPackBuilder::CreateAssetPack(elements);
	CachedBuffer buffer = AssetPack::ToBuffer(pack);

	HZR_ASSERT(pack.ElementCount < 2000, "Too many elements");
	return buffer;
}
