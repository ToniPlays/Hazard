#pragma once

#include "HazardProject.h"
#include "Hazard.h"

namespace Project {

	class ProjectManager : public Hazard::Module::Module {
	public:
		ProjectManager();
		~ProjectManager();

		bool Load(const char* file);
		void Save();
		void SaveCurrentScene();

		HazardProject& GetProject() { return *projectData; }

	private:
		HazardProject* projectData;
	};
}