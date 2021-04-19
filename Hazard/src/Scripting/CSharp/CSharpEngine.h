#pragma once

#include "Hazard/Scripting/ScriptEngine.h"
#include "Mono/Mono.h"

extern "C" 
{
	typedef struct _MonoMethod MonoMethod;
}

namespace Hazard::Scripting::CSharp {

	struct MonoEntity {

		uint32_t handle;
		ScriptData data;
		MonoMethod* Constructor = nullptr;
		MonoMethod* OnCreate = nullptr;
		MonoMethod* OnStart = nullptr;
		MonoMethod* OnUpdate = nullptr;
		MonoMethod* OnLateUpdate = nullptr;
		MonoMethod* OnFixedUpdate = nullptr;

		MonoEntity() = default;

		MonoEntity(ScriptData data) : data(data) {

			Constructor = Mono::GetCoreMethod("Hazard.Entity:.ctor(ulong)");
			OnCreate = Mono::GetAppMethod(std::string(data.name) + ":OnCreate()");
			OnStart = Mono::GetAppMethod(std::string(data.name) + ":Start()");
			OnUpdate = Mono::GetAppMethod(std::string(data.name) + ":OnUpdate(single)");
			OnLateUpdate = Mono::GetAppMethod(std::string(data.name) + ":OnLateUpdate(single)");
			OnFixedUpdate = Mono::GetAppMethod(std::string(data.name) + ":OnFixedUpdate(single)");
		};
	};

	class CSharpEngine : public ScriptEngine {
	public:

		CSharpEngine();

		void OnBeginRuntime() override;
		void OnEndRuntime() override;

		bool ModuleExists(const char* name) override;

		void UpdateEntities() override;

		void OnSceneLoaded() override;
		void OnSceneUnloaded() override;

		//Entity creation
		void InitializeEntity(uint32_t entity, std::string moduleName) override;
		void ClearEntity(uint32_t entity, std::string moduleName) override;
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
		std::unordered_map<uint32_t, ScriptData> scripts;
		std::unordered_map<uint32_t, MonoEntity> entities;
	};
}