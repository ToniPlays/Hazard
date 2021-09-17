#pragma once

#include "Hazard/Core/Core.h"
#include "PublicField.h"

namespace Hazard::Scripting
{
	class ScriptEngine {
	public:

		virtual void OnBeginRuntime() = 0;
		virtual void OnEndRuntime() = 0;
		virtual void UpdateEntities() = 0;

		virtual void OnWorldLoaded() = 0;
		virtual void OnWorldUnloaded() = 0;

		virtual bool ModuleExists(const char* moduleName) = 0;

		virtual std::unordered_map<std::string, PublicField*> GetPublicFields(uint32_t entity, const std::string& moduleName) = 0;
		virtual void InitializeEntity(uint32_t entity, const std::string& moduleName) = 0;
		virtual void Instantiate(uint32_t entity, const std::string& moduleName) = 0;
		virtual void ClearEntity(uint32_t entity, const std::string& moduleName) = 0;

		//Entity updates
		virtual void OnFixedUpdate(uint32_t entity) = 0;
		//Entity remove
		virtual void OnEnable(uint32_t entity) = 0;
		virtual void OnDisable(uint32_t entity) = 0;
		virtual void OnDestroy(uint32_t entity) = 0;
	
		virtual void OnApplicationClose() = 0;

		virtual void Reload() = 0;
	};
}