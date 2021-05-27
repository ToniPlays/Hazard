#pragma once

#include <hzreditor.h>
#include "ProjectManager.h"
#include "GUI/EditorView.h"
#include "GUI/Window/Inspect/FileView.h"

#include <yaml-cpp/yaml.h>

namespace Project {

	ProjectManager::ProjectManager() : Module::Module("ProjectManager")
	{

	}

	ProjectManager::~ProjectManager()
	{

	}

	bool ProjectManager::Load(const std::string& path)
	{
		HazardProject* project = new HazardProject();
		std::ifstream stream(path);
		if (!stream.good()) 
			return false;

		YAML::Node root = YAML::LoadFile(path);
		
		project->m_Name = root["project_name"].as<std::string>();
		project->m_AbsolutePath = root["project_path"].as<std::string>();
		project->m_StartupScene = root["startup_scene"].as<std::string>();
		m_ProjectData = project;

		HZR_INFO("Poject loaded: {0}", path);

		Application::GetModule<WindowElement::EditorView>()->GetRenderable<WindowElement::FileView>()->
			SetRootPath(project->m_AbsolutePath.c_str());

		if (project->m_StartupScene != "") {
			Application::GetModule<ECS::WorldHandler>()->LoadScene(project->m_StartupScene);
		}
		return true;
	}

	void ProjectManager::Save()
	{
		HZR_INFO("Saving TODO");
	}
	void ProjectManager::SaveCurrentScene()
	{
		ECS::World& world = ECS::WorldCommand::GetCurrentWorld();
		ECS::Loader::WorldSerializer::SerializeEditor(world.GetWorldFile().c_str(), world);
	}
}