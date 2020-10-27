#pragma once

#include <hzreditor.h>
#include "HazardProject.h"
#include "Analytics/Debug.h"
#include <yaml-cpp/yaml.h>

HazardProject* HazardProject::current;

HazardProject::HazardProject()
{

}

HazardProject* HazardProject::LoadProject(std::string& path) {
	std::ifstream stream(path);
	std::stringstream ss;

	ss << stream.rdbuf();

	HazardProject* project = new HazardProject();
	
	YAML::Node data = YAML::Load(ss.str());
	if (!data["Hazard project"]) return nullptr;
	project->name = data["Hazard project"].as<std::string>();

	if (!data["Project path"]) return nullptr;
	project->path = data["Project path"].as<std::string>();

	if (!data["Startup Scene"]) return nullptr;
	project->startUpScene = data["Startup Scene"].as<std::string>() + ".hazard";

	return project;
}
void HazardProject::OpenProject(std::string& path)
{
	current = LoadProject(path);
	if (current == nullptr) {
		HZR_CORE_ERROR("Project failed to load");
		return;
	}
	Application::GetApplication().SetTitle(current->name);
	Hazard::SceneManager* manager = Hazard::ModuleHandler::GetModule<SceneManager>();
	manager->LoadScene(current->path + "/" + current->startUpScene);
}
bool HazardProject::SaveProject(std::string& path, HazardProject* project) {

	HZR_ASSERT(false, "Not implemented");
	return false;
}

void HazardProject::SaveCurrentScene()
{
	if (current == nullptr) return;
	Hazard::SceneManager* manager = Hazard::ModuleHandler::GetModule<SceneManager>();
	Hazard::Serializer::SerializeScene(current->path + "/" + current->startUpScene, *manager->GetActiveScene());
}

void HazardProject::NewProject(HazardProject* project)
{
	std::string& path = Hazard::File::SaveFolderDialog();
	if (path == "") {
		HZR_CORE_WARN("Cancelled");
		return;
	}
	SaveProject(path, project);
}
