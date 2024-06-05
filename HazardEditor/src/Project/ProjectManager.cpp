
#include "ProjectManager.h"
#include "Hazard.h"

void ProjectManager::Close()
{
	if (!m_Project.IsValid()) return;

	//m_Project.SerializeSetting<HazardProjectInfo>();
}

void ProjectManager::LoadProjectFromFile(const std::filesystem::path& path)
{
	HazardProject project(path);

	if (!project.IsValid())
		return;
    
	project.ReloadSettings();
	project.ProcessAssets();


	m_Project = project;
}
