
#include "ProjectManager.h"

HazardProject& ProjectManager::LoadProjectFromFile(const std::filesystem::path& path)
{
	HazardProject project(path);
	HZR_ASSERT(project.IsValid(), "We have no project");

	project.ProcessAssets();

	m_Project = project;
	return m_Project;
}
