
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

	if (root["General"]) {
		DeserializeGeneral(root["General"]);
	}
	m_Data.ProjectDirectory = File::GetDirectoryOf(path);
}

void HazardProject::ProcessAssets()
{
	std::filesystem::path libraryPath = m_Data.ProjectDirectory / "Library";
	std::filesystem::path assetPath = m_Data.ProjectDirectory / "Assets";

	return;

	for (auto& item : File::GetAllInDirectory(assetPath))
		ProcessAsset(item);
}

AssetMetadata HazardProject::ProcessAsset(const std::filesystem::path& path)
{
	if (File::GetFileExtension(path) == "meta")
	{
		AssetMetadata metadata = EditorAssetManager::ImportFromMetadata(path);
		std::filesystem::path assetPath = File::GetPathNoExt(path);
		if (File::IsDirectory(assetPath))
		{
			ProcessSubFolderAssets(assetPath);
		}
		return metadata;
	}
	return Hazard::AssetMetadata();
}

void HazardProject::DeserializeGeneral(const YAML::Node& node)
{
	YamlUtils::Deserialize(node, "Project name", m_Data.ProjectName, std::string("New project"));
	YamlUtils::Deserialize(node, "Startup world", m_Data.StartupWorld, std::filesystem::path(""));
}

void HazardProject::ProcessSubFolderAssets(const std::filesystem::path& dir)
{
	for (auto& item : File::GetAllInDirectory(dir))
	{
		if (File::GetFileExtension(item) == "meta")
		{
			EditorAssetManager::ImportFromMetadata(item);

			std::filesystem::path assetPath = File::GetPathNoExt(item);
			if (File::IsDirectory(assetPath)) {
				ProcessSubFolderAssets(assetPath);
			}
		}
	}
}
