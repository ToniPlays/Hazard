#pragma once

#include "Hazard/Module.h"
#include "HazardProject.h"

class ProjectManager : public Hazard::Module {
public:
	ProjectManager() : Module("ProjectManager") {
		s_Instance = this;
	}
	~ProjectManager() = default;
	
	HazardProject& LoadProjectFromFile(const std::filesystem::path& path);

	static bool IsProjectLoaded() { return s_Instance->m_Project.IsValid(); }
	static HazardProject& GetProject() { return s_Instance->m_Project; }
	static std::filesystem::path GetAssetFolder() { return GetProject().GetProjectData().ProjectDirectory.string() + "/Assets"; }
	static std::string GetProjectName() {
		return IsProjectLoaded() ? GetProject().GetProjectData().ProjectName : "Empty project";
	}

private:
	inline static ProjectManager* s_Instance;
	HazardProject m_Project;
};