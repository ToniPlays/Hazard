#pragma once

#include <hzreditor.h>
#include "ProjectManager.h"

#include <yaml-cpp/yaml.h>

namespace Project {

	ProjectManager::ProjectManager() : Module::Module("ProjectManager")
	{
		std::cout << "ProjectManager()" << std::endl;
	}

	ProjectManager::~ProjectManager()
	{

	}

	bool ProjectManager::Load(const char* file)
	{
		HazardProject* project = new HazardProject();
		std::ifstream stream(file);
		if (!stream.good()) 
			return false;

		YAML::Node root = YAML::LoadFile(file);
		
		project->name = root["project_name"].as<std::string>();
		project->absolutePath = root["project_path"].as<std::string>();
		project->startupScene = root["startup_scene"].as<std::string>();

		projectData = project;

		return true;
	}

	void ProjectManager::Save()
	{
		HZR_INFO("Saving TODO");
	}
	void ProjectManager::SaveCurrentScene()
	{
		ECS::World& world = ECS::SceneCommand::GetCurrentWorld();
		ECS::Loader::SceneSerializer::SerializeEditor(world.GetWorldFile().c_str(), world);
	}
}