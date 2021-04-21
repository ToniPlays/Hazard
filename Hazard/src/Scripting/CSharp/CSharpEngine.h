#pragma once

#include "Hazard/Scripting/ScriptEngine.h"
#include "Mono/Mono.h"
#include "Hazard/Entity/World.h"

extern "C" 
{
	typedef struct _MonoMethod MonoMethod;
}

namespace Hazard::Scripting::CSharp {

	struct EntityScript;
	class CSharpField;
	struct EntityInstance
	{
		EntityScript* ScriptClass;
		uint32_t handle = 0;
		ECS::World* instance = nullptr;
		MonoObject* GetInstance() { return Mono::ObjectFromHandle(handle); }
	};
	using ModuleFieldMap = std::unordered_map<std::string, std::unordered_map<std::string, CSharpField*>>;
	struct EntityInstanceData {
		EntityInstance instance;
		ModuleFieldMap moduleFieldMap;
	};

	using EntityInstanceMap = std::unordered_map<uint32_t, EntityInstanceData>;

	struct EntityScript {
		std::string moduleName;
		std::string className;
		std::string nameSpace;

		MonoClass* monoClass;
		MonoMethod* Constructor = nullptr;
		MonoMethod* OnCreated = nullptr;
		MonoMethod* OnStart = nullptr;
		MonoMethod* OnUpdate = nullptr;
		MonoMethod* OnLateUpdate = nullptr;
		MonoMethod* OnFixedUpdate = nullptr;

		void InitClassMethods() {
			Constructor = Mono::GetCoreMethod("Hazard.Entity:.ctor(ulong)");
			OnCreated = Mono::GetAppMethod(moduleName + ":OnCreate()");
			OnStart = Mono::GetAppMethod(moduleName + ":Start()");
			OnUpdate = Mono::GetAppMethod(moduleName + ":OnUpdate(single)");
			OnLateUpdate = Mono::GetAppMethod(moduleName + ":LateUpdate()");
			OnFixedUpdate = Mono::GetAppMethod(moduleName + ":FixedUpdate()");
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
		std::unordered_map<std::string, PublicField*> GetPublicFields(uint32_t entity, const std::string& moduleName) override;

		//Entity creation
		void InitializeEntity(uint32_t entity, const std::string& moduleName) override;
		void Instantiate(uint32_t entity, const std::string& moduleName) override;
		void ClearEntity(uint32_t entity, const std::string& moduleName) override;
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

		EntityInstanceData& GetInstanceData(uint32_t entity);

		void OnApplicationClose() override;
		void Reload() override;
	};
}