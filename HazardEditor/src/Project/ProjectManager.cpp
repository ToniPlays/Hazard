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

	bool ProjectManager::Load(const char* file)
	{
		HazardProject* project = new HazardProject();
		std::ifstream stream(file);
		if (!stream.good()) 
			return false;

		YAML::Node root = YAML::LoadFile(file);
		
		project->m_Name = root["project_name"].as<std::string>();
		project->m_AbsolutePath = root["project_path"].as<std::string>();
		project->m_StartupScene = root["startup_scene"].as<std::string>();

		m_ProjectData = project;

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