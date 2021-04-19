#pragma once

#include "Hazard/Scripting/ScriptEngine.h"

namespace Hazard::Scripting::Visual
{
	class HVSEngine : public ScriptEngine {
	public:

		HVSEngine();

		void OnBeginRuntime() override;
		void OnEndRuntime() override;

		bool ModuleExists(const char* name) override;

		void UpdateEntities() override;

		void OnSceneLoaded() override;
		void OnSceneUnloaded() override;

		void InitializeEntity(uint32_t entity, std::string moduleName) override;
		void ClearEntity(uint32_t entity, std::string moduleName) override;

		//Entity creation
		void OnCreate(uint32_t entity) override;
		void OnStart(uint32_t entity) override;
		//Entity updates
		void OnUpdate(uint32_t entity) override;
		void OnLateUpdate(uint32_t entity) override;
		void OnFixedUpdate(uint32_t entity) override;
		//Entity remove
		void OnEnable(uint32_t entity) override;
		void OnDisable(uint32_t entity) override;
		void OnDestroy(uint32_t entity) override;
		void OnCollision(uint32_t entity) override;

		void OnApplicationClose() override;

		void Reload() override;
		ScriptStats& GetStats() override { return stats; };
		ScriptData& GetData(uint32_t entity, const std::string& moduleName) override;

	private:
		ScriptStats stats;
	};
}