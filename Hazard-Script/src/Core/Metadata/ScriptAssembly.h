#pragma once

#include "Mono/Core/Mono.h"
#include "ScriptMetadata.h"
#include "Ref.h"

namespace HazardScript 
{
	struct ReferencedAssembly 
	{
		std::string Name;
	};

	class ScriptAssembly : public RefCount
	{
	public:
		ScriptAssembly() = default;
		~ScriptAssembly();

		bool LoadFromSource(bool registerScripts = false, bool withReferenced = false);

		void SetSourcePath(const std::filesystem::path& path) { m_Path = path; }
		std::filesystem::path& GetSourcePath() { return m_Path; }

		std::unordered_map<std::string, Ref<ScriptMetadata>>& GetScripts() { return m_Scripts; }
		std::vector<ReferencedAssembly>& GetReferencedAssemblies() { return m_ReferencedAssemblies; }
#ifdef HZR_INCLUDE_MONO
		MonoImage* GetImage() const;
        MonoAssembly* GetAssembly() { return m_Assembly; }
#endif

		bool HasScript(const std::string& name) {
			return m_Scripts.find(name) != m_Scripts.end();
		}
		ScriptMetadata& GetScript(const std::string& name) 
		{
			return *m_Scripts[name];
		}

		template<typename T>
		std::vector<Ref<ScriptMetadata>> ViewAttributes() 
		{
			std::vector<Ref<ScriptMetadata>> results;

			for (auto& [name, script] : m_Scripts) {
				if (script->Has<T>())
					results.push_back(script);
			}
			return results;
		}

	private:
		void LoadReferencedAssemblies();
		void LoadScripts();
		void Release();

	private:
		std::filesystem::path m_Path;
#ifdef HZR_INCLUDE_MONO
		MonoAssembly* m_Assembly = nullptr;
#endif
		std::unordered_map<std::string, Ref<ScriptMetadata>> m_Scripts;
		std::vector<ReferencedAssembly> m_ReferencedAssemblies;
	};
}
