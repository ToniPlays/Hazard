#pragma once

#include "Hazard/Core/Core.h"
#include "Script.h"

namespace Hazard::Scripting 
{
	struct ScriptMetaData 
	{
		std::string ModuleName;
		std::string ClassName;
		std::string Namespace;
		uint32_t Count = 0;
	};

	struct InstanceData 
	{
		uint32_t Handle = 0;
		std::vector<Script*> Scripts;
	};

	class ScriptRegistry 
	{
	public:
		ScriptRegistry() = default;
		
		bool HasScript(const std::string& script);
		bool HasInstance(uint32_t handle);
		ScriptMetaData& GetScriptMetadata(const std::string& script);

		void Add(ScriptMetaData script) { m_Scripts[script.ModuleName] = script; };
		void Remove(uint32_t handle);
		void RegisterEntityScript(uint32_t handle, Script* script);

		InstanceData& GetInstanceData(uint32_t handle) {
			HZR_CORE_ASSERT(m_InstanceData.find(handle) != m_InstanceData.end(), "Handle does not exist");
			return m_InstanceData[handle]; 
		}

		void Clear() { m_Scripts.clear(); }
		std::unordered_map<std::string, ScriptMetaData> GetScripts() { return m_Scripts; }
		std::unordered_map<uint32_t, InstanceData> GetRegisteredInstances() { return m_InstanceData; }

	private:

		std::unordered_map<std::string, ScriptMetaData> m_Scripts;
		std::unordered_map<uint32_t, InstanceData> m_InstanceData;
	};
}