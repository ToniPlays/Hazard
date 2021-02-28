#pragma once

#include "HazardProject.h"
#include "Hazard.h"

namespace Project {

	class ProjectManager : public Hazard::Module::Module {
	public:
		ProjectManager();
		~ProjectManager();

		HazardProject* Load(const char* file);
		void Save();

		HazardProject& GetProject() { return *projectData; }

	private:
		HazardProject* projectData;
	};
}