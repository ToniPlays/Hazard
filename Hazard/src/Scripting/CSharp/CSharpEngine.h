#pragma once

#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/Entity/World.h"

extern "C" 
{
	typedef struct _MonoMethod MonoMethod;
}

namespace Hazard::Scripting::CSharp {

	class CSharpEngine : public ScriptEngine {
	public:

		CSharpEngine(ScriptEngineCreateInfo* info);

		ScriptRegistry& GetRegistry() override { return m_Registry; }

		void OnBeginRuntime() override;
		void OnEndRuntime() override;

		bool ModuleExists(const std::string& name) override;
		void UpdateEntities() override;

		void OnWorldLoaded() override;
		void OnWorldUnloaded() override;

		ScriptField* GetPublicField(uint32_t handle, uint32_t index) override;

		//Entity creation
		void InitializeEntity(uint32_t handle, const std::string& moduleName) override;
		void Instantiate(uint32_t handle, const std::string& moduleName) override;
		void ClearEntity(uint32_t handle, const std::string& moduleName) override;

		//Entity updates
		void OnFixedUpdate(uint32_t handle) override;

		void OnCollided(uint32_t handle, void** param, bool entered) override;
		void OnTrigger(uint32_t handle, void** param, bool entered) override;

		//Entity remove
		void OnEnable(uint32_t handle) override;
		void OnDisable(uint32_t handle) override;
		void OnDestroy(uint32_t handle) override;

		bool EntityInstanceExits(uint32_t handle);

		void OnApplicationClose() override;
		void Reload() override;
	private:
		ScriptRegistry m_Registry;
	};
}
