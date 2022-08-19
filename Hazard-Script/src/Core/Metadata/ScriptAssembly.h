#pragma once

#include "Mono/Core/Mono.h"
#include "ScriptMetadata.h"

namespace HazardScript 
{
	class ScriptAssembly {

	public:
		ScriptAssembly() = default;
		~ScriptAssembly();

		bool LoadFromSource(bool registerScripts = false);

		void SetSourcePath(const std::filesystem::path& path) { m_Path = path; }
		std::filesystem::path& GetSourcePath() { return m_Path; }

		std::unordered_map<std::string, ScriptMetadata*>& GetScripts() { return m_Scripts; }
		MonoImage* GetImage() const;

		bool HasScript(const std::string& name) {
			return m_Scripts.find(name) != m_Scripts.end();
		}
		ScriptMetadata& GetScript(const std::string& name) {
			return *m_Scripts[name];
		}
		MonoAssembly* GetAssembly() { return m_Assembly; }

		template<typename T>
		std::vector<ScriptMetadata*> ViewAttributes() {
			std::vector<ScriptMetadata*> results;

			for (auto& [name, script] : m_Scripts) {
				if (script->Has<T>())
					results.push_back(script);
			}
			return results;
		}

	private:
		void LoadScripts();
	private:
		std::filesystem::path m_Path;
		MonoAssembly* m_Assembly;
		std::unordered_map<std::string, ScriptMetadata*> m_Scripts;
	};
}