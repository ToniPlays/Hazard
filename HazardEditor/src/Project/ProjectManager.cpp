
#include "ProjectManager.h"
#include "Hazard.h"

void ProjectManager::Close()
{
	if (!m_Project.IsValid()) return;

	m_Project.SerializeSetting<HazardProjectInfo>();
	m_Project.SerializeSetting<ProjectInputSettings>();
	m_Project.SerializeSetting<ScriptingSettings>();
}

HazardProject ProjectManager::LoadProjectFromFile(const std::filesystem::path& path)
{
	HazardProject project(path);
    if(!project.IsValid())
        return project;
    
	project.DeserializeSetting<HazardProjectInfo>();
	project.DeserializeSetting<ProjectInputSettings>();
	project.DeserializeSetting<ScriptingSettings>();
	project.ProcessAssets();

	m_Project = project;
	return m_Project;
}
