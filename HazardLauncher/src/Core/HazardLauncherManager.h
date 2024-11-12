#pragma once

#include "UtilityCore.h"
#include "HazardProject.h"

#include "Hazard/Assets/AssetEnums.h"

class HazardLauncherManager 
{
public:
	HazardLauncherManager();
	~HazardLauncherManager() = default;

	bool OpenProject(const HazardProject& project);
	bool ImportProject(const std::filesystem::path& path);
	bool LoadFromConfigFile(const std::filesystem::path& path);
	void SaveConfigToFile(const std::filesystem::path& path);
	bool CreateProject(const HazardProject& project);
    
    const std::filesystem::path& GetInstallationLocation() const { return m_InstallationLocation; }
    bool SetInstallationLocation(const std::filesystem::path& path);

	std::vector<HazardProject> GetProjects() { return m_LoadedProjects; }

private:
	std::vector<HazardProject> m_LoadedProjects;
    std::filesystem::path m_InstallationLocation;
    std::filesystem::path m_ConfigFilePath;
};
