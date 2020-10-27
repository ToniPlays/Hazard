#pragma once
#include "Hazard.h"
#include <yaml-cpp/yaml.h>


class HazardProject {
	
public:
	HazardProject();

	std::string path;
	std::string name;
	std::string startUpScene;

	static void OpenProject(std::string& path);
	static bool SaveProject(std::string& path, HazardProject* project);
	static void SaveCurrentScene();
	static void NewProject(HazardProject* project);
	static HazardProject* current;

private:
	static HazardProject* LoadProject(std::string& path);
};