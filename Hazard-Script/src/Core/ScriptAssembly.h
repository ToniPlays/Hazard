#pragma once

#include "Mono/Core/Mono.h"
#include "Script.h"

namespace HazardScript 
{
	class ScriptAssembly {

	public:
		ScriptAssembly() = default;

		bool LoadFromSource(bool registerScripts = false);

		void SetSourcePath(const std::filesystem::path& path) { m_Path = path; }
		std::filesystem::path& GetSourcePath() { return m_Path; }

		std::unordered_map<std::string, Script>& GetScripts() { return m_Scripts; }
		MonoImage* GetImage();

		bool HasScript(const std::string& name) {
			return m_Scripts.find(name) != m_Scripts.end();
		}
		Script& GetScript(const std::string& name) {
			return m_Scripts[name];
		}
		MonoAssembly* GetAssembly() { return m_Assembly; }

	private:
		void LoadScripts();

	private:
		std::filesystem::path m_Path;
		MonoAssembly* m_Assembly;
		std::unordered_map<std::string, Script> m_Scripts;
	};
}