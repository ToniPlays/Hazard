#pragma once

#include "Hazard/Core/Core.h"

#include "PublicField.h"

extern "C"
{
	typedef struct _MonoClass MonoClass;
}

namespace Hazard::Scripting
{
	struct ScriptStats {
		const char* name;
		const char* assemblyPath;
		
	};

	struct ScriptData
	{
		const char* name;
		std::vector<PublicField*> fields;
		MonoClass* scriptClass = nullptr;

		ScriptData() = default;
		ScriptData(const std::string& name, MonoClass* scriptClass) : name(name.c_str()), scriptClass(scriptClass) {};

	};

	class ScriptEngine {
	public:

		virtual void OnBeginRuntime() = 0;
		virtual void OnEndRuntime() = 0;
		virtual void UpdateEntities() = 0;

		virtual void OnSceneLoaded() = 0;
		virtual void OnSceneUnloaded() = 0;

		virtual bool ModuleExists(const char* moduleName) = 0;
	

		virtual void InitializeEntity(uint32_t entity, std::string moduleName) = 0;
		virtual void ClearEntity(uint32_t entity, std::string moduleName) = 0;
		//Entity creation
		virtual void OnCreate(uint32_t entity) = 0;
		virtual void OnStart(uint32_t entity) = 0;
		//Entity updates
		virtual void OnUpdate(uint32_t entity) = 0;
		virtual void OnLateUpdate(uint32_t entity) = 0;
		virtual void OnFixedUpdate(uint32_t entity) = 0;
		//Entity remove
		virtual void OnEnable(uint32_t entity) = 0;
		virtual void OnDisable(uint32_t entity) = 0;
		virtual void OnDestroy(uint32_t entity) = 0;
		virtual void OnCollision(uint32_t entity) = 0;
	
		virtual void OnApplicationClose() = 0;

		virtual void Reload() = 0;
		virtual ScriptStats& GetStats() = 0;
		virtual ScriptData& GetData(uint32_t entity, const std::string& moduleName) = 0;
	};
}