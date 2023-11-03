
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
		{ "Default",			"Library/Packed/textureBG.png.hpack"},
		{ "Folder",				"Library/Packed/folder.png.hpack"},
		{ "World",				"Library/Packed/world.png.hpack"},
		{ "Script",				"Library/Packed/csharp.png.hpack"},
		{ "Camera",				"Library/Packed/camera.png.hpack"},
		{ "DirectionalLight",	"Library/Packed/directionalLight.png.hpack"}
	};
	std::vector<EditorAsset> meshesToLoad = {
		//{ "Cube", "Library/cube.obj.hpack"     },
		//{ "Sphere", "Library/sphere.obj.hpack" }
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

void EditorAssetManager::SaveAssetsToPack(const std::string& name, std::vector<AssetPackElement> assets)
{
	if (assets.size() == 0) return;

	FileCache cache("Library");

	AssetPack pack = {};
	pack.Handle = AssetHandle();
	pack.ElementCount = assets.size();
	pack.Elements = assets;

	for (auto& e : pack.Elements)
		e.AssetPackHandle = pack.Handle;

	CachedBuffer buffer = AssetPack::ToBuffer(pack);

	if (!cache.WriteFile("Packed" / std::filesystem::path(name + ".hpack"), buffer.GetData(), buffer.GetSize()))
		pack.Free();
}

void EditorAssetManager::ImportEngineAssets()
{
	using namespace Hazard;
	HZR_INFO("Importing engine assets");

	Timer timer;
	FileCache cache("Library/Packed");

	struct LoadingJobs
	{
		std::filesystem::path File;
		JobPromise Promise;
	};

	std::vector<LoadingJobs> promises;

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

		promises.push_back({ file, AssetManager::DataFromSource(file) });
	}

	for (auto& promise : promises)
	{
		promise.Promise.Wait();
		auto results = promise.Promise.GetResults<AssetPackElement>();
		SaveAssetsToPack(File::GetName(promise.File), results);
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

	HZR_INFO("Engine assets imported in {0} ms", timer.ElapsedMillis());
}