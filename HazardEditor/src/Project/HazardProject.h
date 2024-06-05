#pragma once

#include "UtilityCore.h"
#include "Utility/YamlUtils.h"
#include "Hazard/Assets/Asset.h"
#include "Directory.h"

#include "ProjectSettingsSchema.h"
#include "File.h"

class HazardProject
{
public:
	HazardProject() = default;
	HazardProject(const std::filesystem::path& path);

	bool IsValid() { return !m_Info.RuntimeConfig.ProjectPath.empty(); }

	HazardProjectInfo& GetSettings() { return m_Info; }

	std::string GetBuildTargetName() const;

	void ReloadSettings();
	void ProcessAssets();
	void ProcessAsset(const std::filesystem::path& path);

private:
	HazardProjectInfo m_Info;
};