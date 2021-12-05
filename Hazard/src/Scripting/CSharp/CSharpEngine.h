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
		EntityScript* ScriptClass = nullptr;
		uint32_t handle = 0;
		ECS::World* instance = nullptr;
		MonoObject* GetInstance() { return Mono::ObjectFromHandle(handle); }
	};

	using PublicFieldMap = std::unordered_map<std::string, CSharpField*>;
	using ModuleFieldMap = std::unordered_map<std::string, PublicFieldMap>;


	struct EntityScript 
	{
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
		MonoMethod* OnColliderEnter2D = nullptr;
		MonoMethod* OnTriggerEnter2D = nullptr;
		MonoMethod* OnColliderExit2D = nullptr;
		MonoMethod* OnTriggerExit2D = nullptr;

		void InitClassMethods() 
		{
			Constructor = Mono::GetCoreMethod("Hazard.Entity:.ctor(ulong)");
			OnCreated = Mono::GetAppMethod(moduleName + ":OnCreate()");
			OnStart = Mono::GetAppMethod(moduleName + ":OnStart()");
			OnUpdate = Mono::GetAppMethod(moduleName + ":OnUpdate(single)");
			OnLateUpdate = Mono::GetAppMethod(moduleName + ":LateUpdate()");
			OnFixedUpdate = Mono::GetAppMethod(moduleName + ":FixedUpdate()");
			OnColliderEnter2D = Mono::GetAppMethod(moduleName + ":OnColliderEnter2D(Collider2D)");
			OnTriggerEnter2D = Mono::GetAppMethod(moduleName + ":OnTriggerEnter2D(Collider2D)");
			OnColliderExit2D = Mono::GetAppMethod(moduleName + ":OnColliderExit2D(Collider2D)");
			OnTriggerExit2D = Mono::GetAppMethod(moduleName + ":OnTriggerExit2D(Collider2D)");
		};
	};

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

		std::unordered_map<std::string, PublicField*> GetPublicFields(uint32_t handle, const std::string& moduleName) override;

		//Entity creation
		void InitializeEntity(uint32_t handle, const std::string& moduleName) override;
		void Instantiate(uint32_t handle, const std::string& moduleName) override;
		void ClearEntity(uint32_t handle, const std::string& moduleName) override;
		void OnCreate(EntityInstance& handle);
		void OnStart(EntityInstance& handle);

		//Entity updates
		void OnFixedUpdate(uint32_t handle) override;
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