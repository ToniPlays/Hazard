#pragma once

#include <hzreditor.h>
#include "ProjectManager.h"
#include "GUI/EditorView.h"
#include "GUI/Window/Inspect/FileView.h"
#include "GUI/Window/Tabs/EditorMainTab.h"

#include <yaml-cpp/yaml.h>

namespace Project {

	ProjectManager::ProjectManager() : Module::Module("ProjectManager")
	{
		m_ProjectData = new HazardProject();
	}

	ProjectManager::~ProjectManager()
	{

	}

	void ProjectManager::Close()
	{
		Save();
	}

	bool ProjectManager::Load(const std::string& path)
	{
		HazardProject* project = new HazardProject();
		std::ifstream stream(path);

		if (!stream.good()) 
			return false;

		YAML::Node root = YAML::LoadFile(path);

		YAML::Node general = root["General"];

		project->m_Name = general["Project name"].as<std::string>();
		project->m_AbsolutePath = general["Project path"].as<std::string>();
		project->m_StartupWorld = general["Startup world"].as<std::string>();
		m_ProjectData = project;

		Application::GetModule<WindowElement::EditorView>()->GetRenderable<WindowElement::EditorMainTab>()->GetRenderable<WindowElement::FileView>()->
			SetRootPath(project->m_AbsolutePath.c_str());

		if (project->m_StartupWorld != "") {
			if (!Application::GetModule<ECS::WorldHandler>()->LoadWorld(project->m_StartupWorld), ECS::Serialization::Editor) {
				HZR_WARN("Startup world could not be loaded");
			}
		}
		m_ProjectPath = path;
		return true;
	}

	void ProjectManager::Save()
	{
		if (!ProjectLoaded()) return;

		YAML::Emitter out;
		std::ofstream fout(m_ProjectPath);
		out << YAML::BeginMap;
		out << YAML::Key << "General" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Project name" << YAML::Value << m_ProjectData->m_Name;
		out << YAML::Key << "Project path" << YAML::Value << m_ProjectData->m_AbsolutePath;
		out << YAML::Key << "Startup world" << YAML::Value << m_ProjectData->m_StartupWorld;
		
		out << YAML::EndMap;
		out << YAML::EndMap;
		fout << out.c_str();
		fout.close();
	}
	void ProjectManager::SaveCurrentScene()
	{
		ECS::World& world = ECS::WorldCommand::GetCurrentWorld();
		ECS::Loader::WorldSerializer::SerializeEditor(world.GetWorldFile().c_str(), world);
	}
}