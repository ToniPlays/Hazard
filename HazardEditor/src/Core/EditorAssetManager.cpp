
#include "EditorAssetManager.h"
#include "Hazard/Assets/AssetManager.h"
#include "File.h"
#include "Utility/YamlUtils.h"
#include "Hazard.h"

using namespace Hazard;
AssetMetadata EditorAssetManager::ImportFromMetadata(const std::filesystem::path& path)
{
	AssetMetadata metadata = AssetMetadata();
	if (!File::Exists(path)) return metadata;

	YAML::Node root = YAML::LoadFile(path.string());
	YamlUtils::Deserialize<AssetHandle>(root, "UID", metadata.Handle, INVALID_ASSET_HANDLE);
	YamlUtils::Deserialize<AssetType>(root, "Type", metadata.Type, AssetType::Undefined);
	YamlUtils::Deserialize<std::filesystem::path>(root, "Path", metadata.Path, "");
	Hazard::AssetManager::ImportAsset(metadata.Path, metadata);

	HZR_INFO("Importing from {0}", path.string());

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

	if (!File::NewFile(info.Path, sourceFile)) return false;

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

	Ref<World> world = new World(actualPath);
	WorldSerializer serializer(world);
	if (!serializer.SerializeEditor(world->GetWorldFile())) return false;


	AssetMetadata metadata = {};
	metadata.Handle = AssetHandle();
	metadata.Path = actualPath;
	metadata.Type = AssetType::World;

	return CreateMetadataFile(metadata, actualPath);

	return false;
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
	YAML::Emitter out;

	out << YAML::BeginMap;
	YamlUtils::Serialize(out, "UID", metadata.Handle);
	YamlUtils::Serialize(out, "Type", metadata.Type);
	YamlUtils::Serialize(out, "Path", metadata.Path);
	out << YAML::EndMap;
	File::NewFile(path.string() + ".meta", out.c_str());

	return Hazard::AssetManager::ImportAsset(path, metadata) != INVALID_ASSET_HANDLE;
}