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
	void ProjectManager::Close()
	{
		Save();
	}

	bool ProjectManager::Load(const std::string& path)
	{
		using namespace WindowElement;
		HazardProject* project = new HazardProject();
		std::ifstream stream(path);

		if (!stream.good()) 
		{
			delete project;
			return false;
		}

		YAML::Node root = YAML::LoadFile(path);

		YAML::Node general = root["General"];

		YamlUtils::Deserialize(general, "Project name", project->m_Name);
		YamlUtils::Deserialize(general, "Project path", project->m_AbsolutePath);
		YamlUtils::Deserialize(general, "Startup world", project->m_StartupWorld);
		m_ProjectData = project;

		//Application::GetModule<EditorView>().GetRenderable<EditorMainTab>()->GetRenderable<FileView>()->
		//	SetRootPath(project->m_AbsolutePath.c_str());

		if (!project->m_StartupWorld.empty()) {
			if (!Application::GetModule<ECS::WorldHandler>().LoadWorld(project->m_StartupWorld, ECS::Serialization::Editor)) {
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
		YamlUtils::Map(out, "General", [&]() {
			YamlUtils::Serialize(out, "Project name", m_ProjectData->m_Name);
			YamlUtils::Serialize(out, "Project path", m_ProjectData->m_AbsolutePath);
			YamlUtils::Serialize(out, "Startup world", m_ProjectData->m_StartupWorld);
			});
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