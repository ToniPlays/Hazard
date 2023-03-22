
#include "HazardProject.h"
#include "File.h"
#include "Hazard.h"
#include "Core/EditorAssetManager.h"

using namespace Hazard;

HazardProject::HazardProject(const std::filesystem::path& path)
{
	if (!File::Exists(path))
	{
		HZR_FATAL(path.string());
		HZR_ASSERT(false, "This needs to be fixed");
	}

	YAML::Node root = YAML::LoadFile(path.string());

	if (root["General"])
	{
		DeserializeGeneral(root["General"]);
	}
	m_Data.ProjectDirectory = File::GetDirectoryOf(path);
}

void HazardProject::ProcessAssets()
{
	std::filesystem::path libraryPath = m_Data.ProjectDirectory / "Library";
	std::filesystem::path assetPath = m_Data.ProjectDirectory / "Assets";

	for (auto& item : File::GetAllInDirectory(assetPath, true))
		ProcessAsset(item);
}

void HazardProject::ProcessAsset(const std::filesystem::path& path)
{
	AssetType type = Hazard::Utils::AssetTypeFromExtension(File::GetFileExtension(path));

	if (File::GetFileExtension(path) == ".hpack")
	{
		CachedBuffer buffer = File::ReadBinaryFile(path);
		AssetPack pack = AssetPack::Create(buffer, path);

		for (auto& element : pack.Elements)
			AssetManager::ImportAsset(element, File::GetFileAbsolutePath(path).lexically_normal().string());
	}
	else if(type != AssetType::Folder && type != AssetType::Undefined)
	{
		//Asset is a source asset, requires to be added dynamically

		if (!File::Exists(path.string() + ".meta"))
			return;

		std::filesystem::path packFile = path.string() + ".hpack";

		AssetPackElement element = {};
		element.AssetPack = File::Exists(packFile) ? packFile : "";

		YAML::Node node = YAML::LoadFile(path.string() + ".meta");
		YamlUtils::Deserialize(node, "UID", element.Handle, AssetHandle());
		YamlUtils::Deserialize(node, "Type", element.Type, AssetType::Undefined);

		AssetManager::ImportAsset(element, File::GetFileAbsolutePath(path).lexically_normal().string());
	}
}

void HazardProject::DeserializeGeneral(const YAML::Node& node)
{
	YamlUtils::Deserialize(node, "Project name", m_Data.ProjectName, std::string("New project"));
	YamlUtils::Deserialize(node, "Startup world", m_Data.StartupWorld, std::filesystem::path(""));
}

