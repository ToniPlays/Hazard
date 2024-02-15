
#include "EditorAssetManager.h"
#include "Hazard/Assets/AssetManager.h"
#include <Hazard/RenderContext/ShaderAsset.h>
#include "Hazard.h"
#include "File.h"

#include <FileCache.h>
#include <Hazard/Rendering/Mesh/MeshAssetLoader.h>

using namespace Hazard;

void EditorAssetManager::Init()
{
	std::vector<JobPromise> promises;
	ImportEngineShaders(promises);

	for (auto& promise : promises)
		promise.Wait();
}

void EditorAssetManager::PostInit()
{
	std::vector<JobPromise> promises;
	ImportEngineEnvironments(promises);
	ImportEngineMeshes(promises);
	ImportEngineImages(promises);

	for (auto& promise : promises)
		promise.Wait();
}
void EditorAssetManager::LoadEditorAssets()
{
	struct EditorAsset
	{
		std::string Key;
		std::string Path;
	};

	std::vector<EditorAsset> texturesToLoad = {
		{ "Default",			"res/Icons/textureBG.png"},
		{ "Folder",				"res/Icons/folder.png"},
		{ "World",				"res/Icons/world.png"},
		{ "Script",				"res/Icons/csharp.png"},
		{ "Camera",				"res/Icons/camera.png"},
		{ "DirectionalLight",	"res/Icons/directionalLight.png" }
	};

	std::vector<JobPromise> promises;

	Timer timer;
	for (auto& texture : texturesToLoad)
	{
		CreateAssetSettings settings = {};
		settings.SourcePath = texture.Path;

		JobPromise promise = AssetManager::CreateAssetAsync(AssetType::Image, settings);
		promise.Then([key = texture.Key](JobGraph& graph) {
			Ref<Texture2DAsset> asset = graph.GetResult<Ref<Texture2DAsset>>();
			s_Icons[key] = asset->GetHandle();
			asset->IncRefCount();
		});
		promises.push_back(promise);
	}
	for (auto& promise : promises)
		promise.Wait();

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

void EditorAssetManager::ImportEngineShaders(std::vector<JobPromise>& promises)
{
	FileCache cache("Library/Shaders");

	for (auto& file : Directory::GetAllInDirectory("res/Shaders", true))
	{
		if (File::GetFileExtension(file) != ".glsl") continue;
		auto cacheFile = File::GetNameNoExt(file) + ".hasset";

		if (cache.HasFile(cacheFile))
		{
			AssetManager::Import(cache.Get(cacheFile));
			continue;
		}

		CreateAssetSettings settings = {};
		settings.SourcePath = file;

		JobPromise promise = AssetManager::CreateAssetAsync(AssetType::Shader, settings);
		promise.Then([cache](JobGraph& graph) {
			Ref<Asset> asset = graph.GetResult<Ref<Asset>>();
			if (!asset) return;

			SaveAssetSettings settings = {};
			settings.Flags = ASSET_MANAGER_COMBINE_ASSET | ASSET_MANAGER_SAVE_AND_UPDATE;
			settings.TargetPath = cache.GetCachePath() / (File::GetNameNoExt(asset->GetSourceFilePath()) + ".hasset");

			AssetManager::SaveAsset(asset, settings).Wait();
		});

		promises.push_back(promise);
	}
}

void EditorAssetManager::ImportEngineEnvironments(std::vector<JobPromise>& promises)
{
	FileCache cache("Library/Textures");

	for (auto& file : Directory::GetAllInDirectory("res/Textures", true))
	{
		if (File::GetFileExtension(file) != ".hdr") continue;

		auto cacheFile = File::GetNameNoExt(file) + ".hasset";

		if (cache.HasFile(cacheFile))
		{
			AssetManager::Import(cache.Get(cacheFile));
			continue;
		}

		CreateAssetSettings settings = {};
		settings.SourcePath = file;

		JobPromise promise = AssetManager::CreateAssetAsync(AssetType::EnvironmentMap, settings);

		promise.Then([cache](JobGraph& graph) {
			Ref<Asset> asset = graph.GetResult<Ref<Asset>>();
			if (!asset) return;

			SaveAssetSettings settings = {};
			settings.Flags = ASSET_MANAGER_COMBINE_ASSET | ASSET_MANAGER_SAVE_AND_UPDATE;
			settings.TargetPath = cache.GetCachePath() / (File::GetNameNoExt(asset->GetSourceFilePath()) + ".hasset");

			AssetManager::SaveAsset(asset, settings).Wait();
		});

		promises.push_back(promise);
	}
}

void EditorAssetManager::ImportEngineMeshes(std::vector<JobPromise>& promises)
{
	FileCache cache("Library/Meshes");

	for (auto& file : Directory::GetAllInDirectory("res/Meshes", true))
	{
		auto cacheFile = File::GetNameNoExt(file) + ".hasset";

		if (cache.HasFile(cacheFile))
		{
			AssetManager::Import(cache.Get(cacheFile));
			continue;
		}

		CreateAssetSettings settings = {};
		settings.SourcePath = file;

		JobPromise promise = AssetManager::CreateAssetAsync(AssetType::Mesh, settings);
		promise.Then([cache](JobGraph& graph) {
			Ref<Asset> asset = graph.GetResult<Ref<Asset>>();
			if (!asset) return;

			SaveAssetSettings settings = {};
			settings.Flags = ASSET_MANAGER_COMBINE_ASSET | ASSET_MANAGER_SAVE_AND_UPDATE;
			settings.TargetPath = cache.GetCachePath() / (File::GetNameNoExt(asset->GetSourceFilePath()) + ".hasset");

			AssetManager::SaveAsset(asset, settings).Wait();
		});

		//promises.push_back(promise);
	}
}

void EditorAssetManager::ImportEngineImages(std::vector<JobPromise>& promises)
{
	FileCache cache("Library/Textures");

	for (auto& file : Directory::GetAllInDirectory("res/Textures", true))
	{
		auto cacheFile = File::GetNameNoExt(file) + ".hasset";

		if (cache.HasFile(cacheFile))
		{
			AssetManager::Import(cache.Get(cacheFile));
			continue;
		}

		CreateAssetSettings settings = {};
		settings.SourcePath = file;

		JobPromise promise = AssetManager::CreateAssetAsync(AssetType::Image, settings);
		promise.Then([cache](JobGraph& graph) {
			Ref<Asset> asset = graph.GetResult<Ref<Asset>>();
			if (!asset) return;

			SaveAssetSettings settings = {};
			settings.Flags = ASSET_MANAGER_COMBINE_ASSET | ASSET_MANAGER_SAVE_AND_UPDATE;
			settings.TargetPath = cache.GetCachePath() / (File::GetNameNoExt(asset->GetSourceFilePath()) + ".hasset");

			AssetManager::SaveAsset(asset, settings).Wait();
		});

		//promises.push_back(promise);
	}
}

