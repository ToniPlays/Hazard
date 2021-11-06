#pragma once

#include "HazardProject.h"
#include "Hazard.h"

namespace Project {

	class ProjectManager : public Hazard::Module::Module {
	public:
		ProjectManager();
		~ProjectManager() = default;

		void Close() override;

		bool Load(const std::string& path);
		void Save();

		void SaveCurrentWorld();
		bool ProjectLoaded() { return m_ProjectData != nullptr; }
		HazardProject& GetProject() { return *m_ProjectData; }

		template<typename T>
		void Set(const char* key, T value) {
			static_assert(false);
		}
		template<>
		void Set(const char* key, const char* value) {
			if (strcmp(key, "Editor.StartupWorld") == 0) {
				m_ProjectData->StartupWorld = value;
				return;
			}
		}
	private:
		std::string m_ProjectPath;
		HazardProject* m_ProjectData;
	};
}