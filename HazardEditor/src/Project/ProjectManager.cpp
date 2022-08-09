
#include "ProjectManager.h"
#include "Hazard.h"

HazardProject& ProjectManager::LoadProjectFromFile(const std::filesystem::path& path)
{
	HazardProject project(path);
	HZR_INFO("Loading project from {0}", path.string());
	HZR_ASSERT(project.IsValid(), "We have no project");
	project.ProcessAssets();
	
	HZR_ERROR("Trying to make this crap work");
	m_Project = project;
	return m_Project;
}
