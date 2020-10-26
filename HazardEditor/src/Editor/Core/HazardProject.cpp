#pragma once

#include <hzreditor.h>
#include "HazardProject.h"
#include "Analytics/Debug.h"
#include <yaml-cpp/yaml.h>

HazardProject::HazardProject()
{

}

HazardProject HazardProject::LoadProject(std::string& path) {
	std::ifstream stream(path);
	std::stringstream ss;

	ss << stream.rdbuf();

	HazardProject project;

	YAML::Node data = YAML::Load(ss.str());
	if (!data["Hazard project"]) return HazardProject();
	project.name = data["Hazard project"].as<std::string>();

	if (!data["Project path"]) return HazardProject();
	project.path = data["Project path"].as<std::string>();
	if (!data["Startup Scene"]) return HazardProject();
	project.startUpScene = data["Startup Scene"].as<std::string>() + ".hazard";

	return project;
}
void HazardProject::OpenProject(std::string& path)
{
	HazardProject project = LoadProject(path);
	Application::GetApplication().SetTitle(project.name);
	Hazard::SceneManager* manager = Hazard::ModuleHandler::GetModule<SceneManager>();
	manager->LoadScene(project.path + "/" + project.startUpScene);
}
bool HazardProject::SaveProject(std::string& path, HazardProject project) {
	HZR_ASSERT(false, "Not implemented");
	return false;
}