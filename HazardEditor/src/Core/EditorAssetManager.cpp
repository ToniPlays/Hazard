
#include "EditorAssetManager.h"
#include "Hazard/Assets/AssetManager.h"
#include "File.h"
#include "Utility/YamlUtils.h"

using namespace Hazard;
AssetMetadata EditorAssetManager::ImportFromMetadata(const std::filesystem::path& path)
{
	AssetMetadata metadata = AssetMetadata();
	if (!File::Exists(path)) return metadata;

	YAML::Node root = YAML::LoadFile(path.string());
	YamlUtils::Deserialize<AssetHandle>(root, "UID", metadata.Handle, INVALID_ASSET_HANDLE);
	YamlUtils::Deserialize<AssetType>(root, "Type", metadata.Type, AssetType::Undefined);
	YamlUtils::Deserialize<std::filesystem::path>(root, "Path", metadata.Path, "");
	return metadata;
}

void EditorAssetManager::CreateAsset(const AssetType& type, const std::filesystem::path& path)
{
	if (!CreateSourceAsset(type, path)) {
		return;
	}
	AssetMetadata metadata = {};
	metadata.Handle = AssetHandle();
	metadata.Path = path;
	metadata.Type = type;

	YAML::Emitter out;

	out << YAML::BeginMap;
	YamlUtils::Serialize(out, "UID", metadata.Handle);
	YamlUtils::Serialize(out, "Type", metadata.Type);
	YamlUtils::Serialize(out, "Path", metadata.Path);
	out << YAML::EndMap;

	File::NewFile(path.string() + ".meta", out.c_str());

	Hazard::AssetManager::ImportAsset(path, metadata);
}

bool EditorAssetManager::CreateSourceAsset(const AssetType& type, const std::filesystem::path& path)
{
	switch (type)
	{
	case AssetType::Folder:             return File::CreateDir(path);
	case AssetType::AudioClip:          return false;
	case AssetType::World:              return false;
	case AssetType::Image:              return false;
	case AssetType::Mesh:               return false;
	case AssetType::Script:             return false;
	case AssetType::EnvironmentMap:     return false;
	case AssetType::PhysicsMaterial:    return false;
	default:                            return false;
	}
	return false;
}
