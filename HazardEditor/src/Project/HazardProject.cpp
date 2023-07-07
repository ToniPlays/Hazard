
#include "HazardProject.h"
#include "File.h"
#include "Directory.h"
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
		DeserializeGeneral(root["General"]);

	m_Data.ProjectDirectory = File::GetDirectoryOf(path);
}

void HazardProject::ProcessAssets()
{
	std::filesystem::path libraryPath = m_Data.ProjectDirectory / "Library";
	std::filesystem::path assetPath = m_Data.ProjectDirectory / "Assets";

	for (auto& item : Directory::GetAllInDirectory(assetPath, true))
		ProcessAsset(item);
}

void HazardProject::ProcessAsset(const std::filesystem::path& path)
{
	AssetType type = Hazard::Utils::AssetTypeFromExtension(File::GetFileExtension(path));

	if (File::GetFileExtension(path) != ".hpack") return;

	CachedBuffer buffer = File::ReadBinaryFile(path);
	AssetPack pack = AssetPack::Create(buffer);
	AssetManager::ImportAssetPack(pack, path);
	pack.Free();
}

void HazardProject::DeserializeGeneral(const YAML::Node& node)
{
	YamlUtils::Deserialize(node, "Project name", m_Data.ProjectName, std::string("New project"));
	YamlUtils::Deserialize(node, "Startup world", m_Data.StartupWorld, std::filesystem::path(""));
}

