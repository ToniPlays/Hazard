
#include "EditorAssetManager.h"
#include "Hazard/Assets/AssetManager.h"
#include <Hazard/RenderContext/ShaderAsset.h>
#include "Hazard.h"
#include "File.h"

#include <FileCache.h>
#include <Hazard/Rendering/Mesh/MeshAssetLoader.h>
#include <Hazard/RenderContext/ImageAssetLoader.h>

using namespace Hazard;

void EditorAssetManager::Init()
{
	ImportEngineShaders();
	ImportEngineImages();
}

void EditorAssetManager::PostInit()
{
	ImportEngineEnvironments();
	ImportEngineMeshes();
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

	std::vector<Promise<Ref<Texture2DAsset>>> promises;

	Timer timer;
	for (auto& texture : texturesToLoad)
	{
		CreateAssetSettings settings = {};
		settings.SourcePath = texture.Path;

        auto promise = AssetManager::CreateAssetAsync<Texture2DAsset>(AssetType::Image, settings);
        promise.ContinueWith([key = texture.Key](std::vector<Ref<Texture2DAsset>> result) {
            Ref<Texture2DAsset> asset = result[0];
			s_Icons[key] = asset->GetHandle();
			asset->IncRefCount();
        }).Catch([texture](const JobException& e) {
            HZR_ERROR("Failed to load {} with {}", texture.Path, e.what());
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

void EditorAssetManager::ImportEngineShaders()
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

        Promise<Ref<ShaderAsset>> promise = AssetManager::CreateAssetAsync<ShaderAsset>(AssetType::Shader, settings);
		promise.ContinueWith([cache](std::vector<Ref<ShaderAsset>> results) {
            Ref<Asset> asset = results[0];
			if (!asset) return;

			SaveAssetSettings settings = {};
			settings.Flags = ASSET_MANAGER_COMBINE_ASSET | ASSET_MANAGER_SAVE_AND_UPDATE;
			settings.TargetPath = cache.GetCachePath() / (File::GetNameNoExt(asset->GetSourceFilePath()) + ".hasset");

			//AssetManager::SaveAsset(asset, settings).Wait();
		});
	}
}

void EditorAssetManager::ImportEngineEnvironments()
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

        Promise<Ref<EnvironmentMap>> promise = AssetManager::CreateAssetAsync<EnvironmentMap>(AssetType::EnvironmentMap, settings);

        promise.ContinueWith([cache](std::vector<Ref<EnvironmentMap>> results) {
            Ref<Asset> asset = results[0];
			if (!asset) return;

			SaveAssetSettings settings = {};
			settings.Flags = ASSET_MANAGER_COMBINE_ASSET | ASSET_MANAGER_SAVE_AND_UPDATE;
			settings.TargetPath = cache.GetCachePath() / (File::GetNameNoExt(asset->GetSourceFilePath()) + ".hasset");

			//AssetManager::SaveAsset(asset, settings).Wait();
		});
	}
}

void EditorAssetManager::ImportEngineMeshes()
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

        Promise<Ref<Mesh>> promise = AssetManager::CreateAssetAsync<Mesh>(AssetType::Mesh, settings);
		promise.ContinueWith([cache](std::vector<Ref<Mesh>> results) {
            Ref<Asset> asset = results[0];
			if (!asset) return;

			SaveAssetSettings settings = {};
			settings.Flags = ASSET_MANAGER_COMBINE_ASSET | ASSET_MANAGER_SAVE_AND_UPDATE;
			settings.TargetPath = cache.GetCachePath() / (File::GetNameNoExt(asset->GetSourceFilePath()) + ".hasset");

			//AssetManager::SaveAsset(asset, settings).Wait();
		});

		//promises.push_back(promise);
	}
}

void EditorAssetManager::ImportEngineImages()
{
	FileCache cache("Library/Textures");

	for (auto& file : Directory::GetAllInDirectory("res/Textures", true))
	{
		if (File::GetFileExtension(file) == ".hdr") continue;

		auto cacheFile = File::GetNameNoExt(file) + ".hasset";

		if (cache.HasFile(cacheFile))
		{
			AssetManager::Import(cache.Get(cacheFile));
			continue;
		}

		Hazard::ImageAssetLoader::CreateSettings spec = {};
		spec.FlipOnLoad = true;

		CreateAssetSettings settings = {};
		settings.SourcePath = file;
		settings.Settings = &spec;

        Promise<Ref<Texture2DAsset>> promise = AssetManager::CreateAssetAsync<Texture2DAsset>(AssetType::Image, settings);
		promise.ContinueWith([cache](std::vector<Ref<Texture2DAsset>> results) {
            Ref<Asset> asset = results[0];
			if (!asset) return;

			SaveAssetSettings settings = {};
			settings.Flags = ASSET_MANAGER_COMBINE_ASSET | ASSET_MANAGER_SAVE_AND_UPDATE;
			settings.TargetPath = cache.GetCachePath() / (File::GetNameNoExt(asset->GetSourceFilePath()) + ".hasset");

			//AssetManager::SaveAsset(asset, settings).Wait();
		});

		//promises.push_back(promise);
	}
}

