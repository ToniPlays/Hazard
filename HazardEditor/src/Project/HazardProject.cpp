
#include "HazardProject.h"
#include "File.h"
#include "Directory.h"
#include "Hazard.h"
#include "Core/EditorAssetManager.h"

using namespace Hazard;

HazardProject::HazardProject(const std::filesystem::path& path)
{
	HZR_ASSERT(File::Exists(path), "Project file does not exist");
	m_Info.ProjectPath = path;
}

void HazardProject::ProcessAssets()
{
	std::filesystem::path libraryPath = m_Info.ProjectPath / "Library";
	std::filesystem::path assetPath = m_Info.ProjectPath / "Assets";

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
