#pragma once

#include "UtilityCore.h"
#include "HazardProject.h"

class HazardLauncherManager {
public:
	HazardLauncherManager() = default;
	~HazardLauncherManager() = default;

	void OpenProject(const HazardProject& project);
	bool ImportProject(const std::filesystem::path& path);
	bool LoadFromConfigFile(const std::filesystem::path& path);
	void SaveConfigToFile(const std::filesystem::path& path);

	std::vector<HazardProject> GetProjects() { return m_LoadedProjects; }

private:
	std::vector<HazardProject> m_LoadedProjects;

};