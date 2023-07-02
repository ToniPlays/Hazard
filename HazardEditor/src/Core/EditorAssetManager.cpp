
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
	}

	for (auto& mesh : meshesToLoad)
	{
		//Get asset pack handle
		AssetHandle packHandle = AssetManager::GetHandleFromKey(mesh.Path);
		AssetPack pack = AssetManager::OpenAssetPack(packHandle);
		s_Icons[mesh.Key] = pack.Elements[0].Handle;
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

void EditorAssetManager::GenerateAndSavePack(Ref<Job> job, std::filesystem::path& path)
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
	Timer timer;
	HZR_INFO("Importing engine assets");

	FileCache cache("Library");

	std::vector<Ref<Job>> jobs;

	for (auto& file : File::GetAllInDirectory("res", true))
	{
		if (File::IsDirectory(file)) continue;

		auto packPath = File::GetName(file) + ".hpack";
		if (cache.HasFile(packPath)) continue;

		Ref<Job> job = Ref<Job>::Create(GenerateAndSavePack, file);
		jobs.push_back(job);
	}

	Ref<JobGraph> loadingGraph = Ref<JobGraph>::Create("EngineLoad", 1);
	loadingGraph->GetStage(0)->QueueJobs(jobs);

	JobSystem& system = Application::Get().GetJobSystem();
	JobPromise<bool> promise = system.QueueGraph<bool>(loadingGraph);

	promise.Wait();
	
	auto files = File::GetAllInDirectory(cache.GetCachePath(), true);
	for (auto& file : files)
	{
		auto packPath = File::GetName(file);

		CachedBuffer buffer = File::ReadBinaryFile(file);
		AssetPack pack = AssetPack::Create(buffer);
		AssetManager::ImportAssetPack(pack, file);
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

		AssetPackElement element = graph->Execute()->GetResult<AssetPackElement>();
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

		AssetPackElement element = EditorAssetPackBuilder::CreatePackElement(path, info, settings)->Execute()->GetResult<AssetPackElement>();
		element.Handle = AssetHandle();
		elements.push_back(element);
		break;
	}
	case AssetType::Mesh:
	{
		MeshCreateInfo info = {};
		info.DebugName = File::GetName(path);

		UI::MeshImportSettings settings = {};

		AssetPackElement element = EditorAssetPackBuilder::CreatePackElement(path, info, settings)->Execute()->GetResult<AssetPackElement>();
		element.Handle = AssetHandle();
		elements.push_back(element);
	}
	}

	AssetPack pack = EditorAssetPackBuilder::CreateAssetPack(elements);
	CachedBuffer buffer = EditorAssetPackBuilder::AssetPackToBuffer(pack);
	return buffer;
}
