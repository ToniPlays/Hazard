
#include "EditorAssetManager.h"
#include "Hazard/Assets/AssetManager.h"
#include "File.h"
#include "Utility/YamlUtils.h"
#include "Hazard.h"
#include <Hazard/RenderContext/ShaderAsset.h>
#include <src/Core/GUIManager.h>
#include <src/GUI/Overlays/ProgressOverlay.h>

using namespace Hazard;


void EditorAssetManager::ImportAssets()
{
	for (auto& file : File::GetAllInDirectory("res/Icons", true))
		AssetManager::ImportAsset(file);
	for (auto& file : File::GetAllInDirectory("res/Mesh", true))
		AssetManager::ImportAsset(file);
	for (auto& file : File::GetAllInDirectory("res/Shaders", true))
		AssetManager::ImportAsset(file);
	for (auto& file : File::GetAllInDirectory("res/Textures", true))
		AssetManager::ImportAsset(file);
}
void EditorAssetManager::Init()
{
	struct EditorTexture
	{
		const char* Key;
		const char* Path;
	};

	std::vector<EditorTexture> texturesToLoad = {
		{ "Default", "res/Icons/textureBG.png"},
		{ "Folder", "res/Icons/folder.png"},
		{ "World", "res/Icons/world.png"},
		{ "Script", "res/Icons/csharp.png"},
		{ "Camera", "res/Icons/camera.png"},
		{ "DirectionalLight", "res/Icons/directionalLight.png"}
	};
	std::vector<JobPromise> promises;
	promises.reserve(texturesToLoad.size());
	for (auto& texture : texturesToLoad)
	{
		auto promise = AssetManager::GetAssetAsync<Texture2DAsset>(texture.Path);
		auto waitPromise = promise.Then([texture](JobSystem* system, Job* job) -> size_t {
			Job* dependency = system->GetJob(job->Dependency);
			s_Icons[texture.Key] = std::move(*dependency->Value<Ref<Texture2DAsset>>());
			return 0;
			});

		promises.push_back(waitPromise);
	}
	for (auto& promise : promises)
		promise.Wait();

	RefreshEditorAssets();
}

AssetMetadata EditorAssetManager::ImportFromMetadata(const std::filesystem::path& path)
{
	AssetMetadata metadata = AssetMetadata();
	if (!File::Exists(path))
		return metadata;

	YAML::Node root = YAML::LoadFile(path.string());
	YamlUtils::Deserialize<AssetHandle>(root, "UID", metadata.Handle, INVALID_ASSET_HANDLE);
	YamlUtils::Deserialize<AssetType>(root, "Type", metadata.Type, AssetType::Undefined);
	YamlUtils::Deserialize<std::filesystem::path>(root, "Path", metadata.Path, "");

	Hazard::AssetManager::ImportAsset(metadata.Path, metadata);

	return metadata;
}

bool EditorAssetManager::CreateScriptAsset(const ScriptCreateInfo& info)
{
	HZR_ASSERT(!info.ClassName.empty(), "Class name cannot be empty");

	std::string methodList = "";

	for (auto& [key, method] : info.Methods) {
		methodList += method + "\n";
	}

	std::string sourceFile = File::ReadFile("res/ScriptTemplate/TemplateScript.cs");
	sourceFile = StringUtil::Replace(sourceFile, "%ScriptName%", info.ClassName);
	sourceFile = StringUtil::Replace(sourceFile, "%DerivesFrom%", info.Derives.empty() ? "" : ": " + info.Derives);
	sourceFile = StringUtil::Replace(sourceFile, "%MethodList%", methodList);

	if (!File::WriteFile(info.Path, sourceFile)) return false;

	AssetMetadata metadata = {};
	metadata.Handle = AssetHandle();
	metadata.Path = info.Path;
	metadata.Type = AssetType::Script;

	return CreateMetadataFile(metadata, info.Path);
}

bool EditorAssetManager::CreateWorld(const std::filesystem::path& path)
{
	std::filesystem::path actualPath = path;

	if (File::Exists(actualPath)) {
		size_t i = 1;
		std::filesystem::path curPath = File::AppendToName(actualPath, std::to_string(i));
		while (File::Exists(curPath))
		{
			curPath = File::AppendToName(actualPath, std::to_string(i));
			i++;
		}
		actualPath = curPath;
	}

	Ref<World> world = Ref<World>::Create(actualPath);
	WorldSerializer serializer(world);
	if (!serializer.SerializeEditor(world->GetWorldFile())) return false;


	AssetMetadata metadata = {};
	metadata.Handle = AssetHandle();
	metadata.Path = actualPath;
	metadata.Type = AssetType::World;

	return CreateMetadataFile(metadata, actualPath);
}

bool EditorAssetManager::CreateFolder(const std::filesystem::path& path)
{
	std::filesystem::path actualPath = path;
	if (File::DirectoryExists(actualPath)) {
		size_t i = 1;
		std::filesystem::path curPath = File::AppendToName(actualPath, std::to_string(i));
		while (File::DirectoryExists(curPath))
		{
			curPath = File::AppendToName(actualPath, std::to_string(i));
			i++;
		}
		actualPath = curPath;
	}
	if (!File::CreateDir(actualPath)) return false;

	AssetMetadata metadata = {};
	metadata.Handle = AssetHandle();
	metadata.Path = actualPath;
	metadata.Type = AssetType::Folder;

	return CreateMetadataFile(metadata, actualPath);
}

bool EditorAssetManager::CreateMetadataFile(const AssetMetadata& metadata, const std::filesystem::path& path)
{
	std::filesystem::path metaPath = path.string() + ".meta";
	if (File::Exists(metaPath)) return false;
	YAML::Emitter out;

	out << YAML::BeginMap;
	YamlUtils::Serialize(out, "UID", metadata.Handle);
	YamlUtils::Serialize(out, "Type", metadata.Type);
	YamlUtils::Serialize(out, "Path", metadata.Path);
	out << YAML::EndMap;
	File::WriteFile(metaPath, out.c_str());

	return Hazard::AssetManager::ImportAsset(path, metadata) != INVALID_ASSET_HANDLE;
}

bool EditorAssetManager::RenameAsset(const std::string& newName, AssetHandle handle)
{
	AssetMetadata& metadata = AssetManager::GetMetadata(handle);
	std::filesystem::path oldAssetPath = metadata.Path;
	std::string extension = File::GetFileExtension(oldAssetPath);
	std::filesystem::path newAssetPath;

	if (metadata.Type == AssetType::Folder)
	{
		return false;
	}
	else
	{
		newAssetPath = File::GetDirectoryOf(oldAssetPath) / (newName + "." + extension);
		File::WriteFile(newAssetPath.string());
		File::Move(oldAssetPath.string(), newAssetPath.string());
	}

	metadata.Path = newAssetPath;

	YAML::Emitter out;
	out << YAML::BeginMap;
	YamlUtils::Serialize(out, "UID", metadata.Handle);
	YamlUtils::Serialize(out, "Type", metadata.Type);
	YamlUtils::Serialize(out, "Path", newAssetPath);
	out << YAML::EndMap;

	File::WriteFile(newAssetPath.string() + ".meta");
	HZR_ASSERT(File::Move(oldAssetPath.string() + ".meta", newAssetPath.string() + ".meta"), "Oops");
	File::WriteFile(newAssetPath.string() + ".meta", out.c_str());


	AssetManager::GetMetadataRegistry()[newAssetPath] = metadata;
	AssetManager::GetMetadataRegistry().erase(oldAssetPath);
	auto& registry = AssetManager::GetMetadataRegistry();

	return false;
}

Ref<Texture2DAsset> EditorAssetManager::GetIcon(const std::string& name)
{
	if (s_Icons.find(name) != s_Icons.end())
		return s_Icons[name];
	return s_Icons["Default"];
}

void EditorAssetManager::RefreshEditorAssets()
{
	//Compile non cached shaders
	std::unordered_map<AssetType, std::vector<Ref<Asset>>> assetsToUpdate;
	{
		for (auto& file : File::GetAllInDirectory("res/Shaders", true))
		{
			switch (Utils::AssetTypeFromExtension(File::GetFileExtension(file)))
			{
			case AssetType::Shader:
			{
				using namespace HazardRenderer;
				Ref<Asset> asset = AssetManager::GetAsset<ShaderAsset>(file);
				auto cacheDir = ShaderCompiler::GetCachedFilePath(file, RenderAPI::OpenGL);

				if (File::IsNewerThan(file, cacheDir))
					assetsToUpdate[AssetType::Shader].push_back(asset);
				break;
			}
			default:
				break;
			}
		}
	}
	auto progressPanel = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<UI::ProgressOverlay>();

	for (auto& [type, assets] : assetsToUpdate)
	{
		std::string tag = Utils::AssetTypeToString(type);

		std::vector<JobPromise> promises;
		promises.reserve(assets.size());

		for (auto& asset : assets)
			promises.push_back(AssetManager::SaveAssetAsync(asset));

		progressPanel->AddProcesses(type, promises);
	}
}
