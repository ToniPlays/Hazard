#pragma once

#include <hzreditor.h>
#include "ProjectManager.h"

#include <yaml-cpp/yaml.h>

namespace Project {

	ProjectManager::ProjectManager() : Module::Module("ProjectManager")
	{
	}

	ProjectManager::~ProjectManager()
	{
	}

	HazardProject* ProjectManager::Load(const char* file)
	{
		HZR_INFO(file);
		return nullptr;
	}

	void ProjectManager::Save()
	{
		HZR_INFO(projectData->absolutePath);
	}
}