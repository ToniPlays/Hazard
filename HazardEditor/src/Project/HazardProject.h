#pragma once

#include "UtilityCore.h"
#include "Utility/YamlUtils.h"
#include "Hazard/Assets/Asset.h"

struct ProjectGeneralData 
{
	std::string ProjectName;
	std::filesystem::path ProjectDirectory;
	std::filesystem::path StartupWorld;
};

class HazardProject 
{
public:

	HazardProject() = default;
	HazardProject(const std::filesystem::path& path);
	bool IsValid() { return !m_Data.ProjectDirectory.empty(); }

	void ProcessAssets();

	Hazard::AssetMetadata ProcessAsset(const std::filesystem::path& path);

	const ProjectGeneralData& GetProjectData() { return m_Data; }

private:
	void DeserializeGeneral(const YAML::Node& node);
	void ProcessSubFolderAssets(const std::filesystem::path& dir);

private:
	ProjectGeneralData m_Data;
};