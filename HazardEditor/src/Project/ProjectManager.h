#pragma once

#include "HazardProject.h"
#include "Hazard.h"

namespace Project {

	class ProjectManager : public Hazard::Module::Module {
	public:
		ProjectManager();
		~ProjectManager();

		bool Load(const std::string& path);
		void Save();
		void SaveCurrentScene();

		HazardProject& GetProject() { return *m_ProjectData; }

	private:
		HazardProject* m_ProjectData;
	};
}