/*#pragma once

#include "Hazard/Scripting/ScriptEngine.h"

namespace Hazard::Scripting::Visual
{
	class HVSEngine : public ScriptEngine {
	public:

		HVSEngine(ScriptEngineCreateInfo* info);

		ScriptRegistry& GetRegistry() { return m_Registry; }

		void OnBeginRuntime() override;
		void OnEndRuntime() override;

		bool ModuleExists(const std::string& name) override;

		void UpdateEntities() override;

		void OnWorldLoaded() override;
		void OnWorldUnloaded() override;

		std::unordered_map<std::string, PublicField*> GetPublicFields(uint32_t entity, const std::string& moduleName) override;
		void InitializeEntity(uint32_t entity, const std::string& moduleName) override;
		void Instantiate(uint32_t entity, const std::string& moduleName);
		void ClearEntity(uint32_t entity, const std::string& moduleName) override;

		//Entity updates
		void OnFixedUpdate(uint32_t entity) override;
		//Entity remove
		void OnEnable(uint32_t entity) override;
		void OnDisable(uint32_t entity) override;
		void OnDestroy(uint32_t entity) override;

		void OnApplicationClose() override;

		void Reload() override;
	private:
		ScriptRegistry m_Registry;
	};
}
*/