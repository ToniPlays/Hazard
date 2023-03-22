
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
		const char* Key;
		const char* Path;
		JobPromise<Ref<Asset>> Promise;
	};

	std::vector<EditorAsset> texturesToLoad = {
		{ "Default", "textureBG.png.hpack"},
		{ "Folder", "folder.png.hpack"},
		{ "World", "world.png.hpack"},
		{ "Script", "csharp.png.hpack"},
		{ "Camera", "camera.png.hpack"},
		{ "DirectionalLight", "directionalLight.png.hpack"}
	};
	std::vector<EditorAsset> meshesToLoad = {
		{ "Cube", "cube.fbx.hpack"     },
		{ "Sphere", "sphere.fbx.hpack" }
	};

	return;

	Timer timer;
	for (auto& texture : texturesToLoad)
		texture.Promise = AssetManager::GetAssetAsync<Asset>(texture.Path);

	for (auto& mesh : meshesToLoad)
		mesh.Promise = AssetManager::GetAssetAsync<Asset>(mesh.Path);

	for (auto& texture : texturesToLoad)
	{
		texture.Promise.Wait();
		s_Icons[texture.Key] = texture.Promise.Result().As<Texture2DAsset>();
	}
	for (auto& mesh : meshesToLoad)
	{
		mesh.Promise.Wait();
		s_DefaultMesh[mesh.Key] = mesh.Promise.Result().As<Mesh>();
	}

	HZR_INFO("Editor assets loaded in {}ms", timer.ElapsedMillis());
}



Ref<Texture2DAsset> EditorAssetManager::GetIcon(const std::string& name)
{
	if (s_Icons.find(name) != s_Icons.end())
		return s_Icons[name];
	return s_Icons["Default"];
}
Ref<Mesh> EditorAssetManager::GetDefaultMesh(const std::string& name)
{
	if (s_DefaultMesh.find(name) != s_DefaultMesh.end())
		return s_DefaultMesh[name];
	return nullptr;
}

void EditorAssetManager::GenerateAndSavePack(Ref<Job> job, std::filesystem::path& path)
{
	FileCache cache("Library");

	auto packPath = File::GetName(path) + ".hpack";
	CachedBuffer buffer = EditorAssetManager::GenerateEngineAssetPack(path);
	if (buffer.GetSize() == 0) return;

	if (!File::WriteBinaryFile(cache.GetCachePath() / packPath, buffer.GetData(), buffer.GetSize()))
		HZR_WARN("Asset pack saving failed");
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
		auto packPath = File::GetName(file) + ".hpack";
		//if (cache.HasFile(packPath)) continue;

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
		AssetPack pack = AssetPack::Create(buffer, file);

		for (auto& element : pack.Elements)
			AssetManager::ImportAsset(element, File::GetName(packPath));
	}
	HZR_INFO("Engine assets imported in {}ms", timer.ElapsedMillis());
}

CachedBuffer EditorAssetManager::GenerateEngineAssetPack(const std::filesystem::path& path)
{
	AssetType type = Hazard::Utils::AssetTypeFromExtension(File::GetFileExtension(path));

	if (type == AssetType::Undefined || type == AssetType::Folder)
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
