#pragma once

#include "Hazard/Module/Module.h"
#include "Hazard/Entity/Scene.h"
#include "ScriptUtils.h"
#include "PublicField.h"


namespace Hazard::Scripting {

	class ScriptEngine : public Module::Module {
	public:
		ScriptEngine();
		~ScriptEngine();

		void Init() override;
		void Update() override;

		void Close() override;
		void InitAssembly(const char* assemblyPath);
		void LoadRuntimeAssembly(const char* path);

		void ReloadRuntimeAssembly();
		void ReloadRuntimeAssembly(const char* assemblyPath);

		bool ModuleExists(const std::string& module);

		void RegisterScripEntity(const std::string& moduleName, uint32_t id);
		void RemoveScriptEntity(const std::string& moduleName, uint32_t id);

		void InstantiateScriptEntity(uint32_t handle, const std::string& moduleName);

		EntityInstanceData& GetInstanceData(uint32_t entity);

		bool GetRunning() { return running; }
		void SetRunning(bool running) { this->running = running; }

		void Step();
		void EntityStart(EntityInstance& instance);

	private:
		void MonoInit();
		void MonoShutdown();
		uint32_t Instantiate(EntityScript& instance);

	private:
		bool running = false;
	};
}