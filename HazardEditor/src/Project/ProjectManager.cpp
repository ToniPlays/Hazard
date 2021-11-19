#pragma once

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

		YamlUtils::Deserialize(general, "Project name", project->Name);
		YamlUtils::Deserialize(general, "Project path", project->AbsolutePath);
		YamlUtils::Deserialize(general, "Startup world", project->StartupWorld);
		m_ProjectData = project;

		Application::GetModule<EditorView>().GetRenderable<EditorMainTab>()->GetRenderable<FileView>()->
			SetRootPath(project->AbsolutePath);

		if (!project->StartupWorld.empty()) {
			if (!Application::GetModule<ECS::WorldHandler>().LoadWorld(project->StartupWorld, ECS::Serialization::Editor)) {
				HZR_WARN("Startup world could not be loaded");
			}
		}

		FolderData data = File::GetFolderFiles(project->AbsolutePath);
		ImportFiles(data.Files);
		ImportFromFolder(data.Folders);

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
			YamlUtils::Serialize(out, "Project name", m_ProjectData->Name);
			YamlUtils::Serialize(out, "Project path", m_ProjectData->AbsolutePath);
			YamlUtils::Serialize(out, "Startup world", m_ProjectData->StartupWorld);
			});
		out << YAML::EndMap;
		fout << out.c_str();
		fout.close();
	}
	void ProjectManager::SaveCurrentWorld()
	{
		Ref<ECS::World> world = ECS::WorldCommand::GetCurrentWorld();
		ECS::Loader::WorldSerializer::SerializeEditor(world->GetWorldFile().c_str(), world);
	}
	void ProjectManager::ImportFiles(const std::vector<std::filesystem::directory_entry>& files)
	{
		for (auto file : files)
		{
			if (strcmp(File::GetFileExtension(file.path().string()).c_str(), "meta"))
				AssetManager::ImportAsset(file.path().string());
		}
	}
	void ProjectManager::ImportFromFolder(const std::vector<std::filesystem::directory_entry>& directories)
	{
		for (auto directory : directories) {
			FolderData data = File::GetFolderFiles(directory.path().string());
			ImportFiles(data.Files);
			ImportFromFolder(data.Folders);
		}
	}
}