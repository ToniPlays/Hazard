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
		HZR_INFO("Saving project to" + std::string(projectData->absolutePath));
	}
	void ProjectManager::SaveCurrentScene()
	{

	}
}