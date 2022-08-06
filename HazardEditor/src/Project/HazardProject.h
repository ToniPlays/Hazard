#pragma once

#include "UtilityCore.h"
#include "Utility/YamlUtils.h"

struct ProjectGeneralData 
{
	std::string ProjectName;
	std::filesystem::path ProjectDirectory = "";
	std::filesystem::path StartupWorld;
};

class HazardProject {
public:

	HazardProject() = default;
	HazardProject(const std::filesystem::path& path);

	bool IsValid() { return !m_Data.ProjectDirectory.empty(); }
	
	void ProcessAssets();

	const ProjectGeneralData& GetProjectData() { return m_Data; }

private:
	void DeserializeGeneral(const YAML::Node& node);
	void ProcessSubFolderAssets(const std::filesystem::path& dir);

private:
	ProjectGeneralData m_Data;
};