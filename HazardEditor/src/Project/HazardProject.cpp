
#include "HazardProject.h"
#include "File.h"
#include "Directory.h"
#include "Hazard.h"
#include "Core/EditorAssetManager.h"

using namespace Hazard;

HazardProject::HazardProject(const std::filesystem::path& path)
{
	//HZR_ASSERT(File::Exists(path), "Project file does not exist");
	m_Info.RuntimeConfig.ProjectPath = path;
}

std::string HazardProject::GetBuildTargetName() const
{
	std::string result = m_Info.BuildSettings.BuildTargetFormat;
	result = std::move(StringUtil::Replace(result, "{project.company}", m_Info.RuntimeConfig.CompanyName));
	result = std::move(StringUtil::Replace(result, "{project.name}", m_Info.RuntimeConfig.ProjectName));
	result = std::move(StringUtil::Replace(result, "{project.version}", m_Info.RuntimeConfig.VersionIdentifier));

	result = std::move(StringUtil::Replace(result, "{build.platform}", "NaNDroid"));

	result = std::move(StringUtil::Replace(result, " ", "_"));
	return result;
}

void HazardProject::ReloadSettings()
{
	auto p = File::GetDirectoryOf(m_Info.RuntimeConfig.ProjectPath);
	m_Info.RuntimeConfig.Load(m_Info.RuntimeConfig.ProjectPath);
	m_Info.BuildSettings.Load(p / "Editor" / "BuildSettings.hzrcfg");
}

void HazardProject::ProcessAssets()
{
	std::filesystem::path libraryPath = m_Info.RuntimeConfig.ProjectPath / "Library";
	std::filesystem::path assetPath = m_Info.RuntimeConfig.ProjectPath / "Assets";

	for (auto& item : Directory::GetAllInDirectory(assetPath, true))
		ProcessAsset(item);
}

void HazardProject::ProcessAsset(const std::filesystem::path& path)
{
	if (File::GetFileExtension(path) != ".hasset" && File::GetFileExtension(path) != ".hpack") return;
	AssetManager::Import(path);
}
