#pragma once

#include "Hazard/Module.h"
#include "HazardProject.h"

class ProjectManager : public Hazard::Module {
public:
	ProjectManager() : Module("ProjectManager") {
		s_Instance = this;
	}
	~ProjectManager() = default;

	void Close() override;
	
	void LoadProjectFromFile(const std::filesystem::path& path);

	static bool IsProjectLoaded() { return s_Instance->m_Project.IsValid(); }
	static HazardProject& GetCurrentProject() { return s_Instance->m_Project; }

private:
	inline static ProjectManager* s_Instance;
	HazardProject m_Project;
	std::vector<std::filesystem::path> m_Projects;
};
