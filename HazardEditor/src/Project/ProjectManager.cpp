
#include "ProjectManager.h"
#include "Hazard.h"

void ProjectManager::Close()
{

}

void ProjectManager::LoadProjectFromFile(const std::filesystem::path& path)
{
	HazardProject project(path);

	if (!project.IsValid())
		return;
    
	project.ProcessAssets();
	project.ReloadSettings();

	m_Project = project;
}
