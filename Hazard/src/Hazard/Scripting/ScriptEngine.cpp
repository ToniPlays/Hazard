#pragma once

#include <hzrpch.h>
#include "ScriptEngine.h"
#include "Hazard/File/File.h"

#include "ScriptUtils.h"
#include "ScriptCommand.h"
#include "Mono/MonoCommands.h"
#include "Script/ScriptRegistry.h"

namespace Hazard::Scripting {

	struct AssemblyData {
		EntityInstanceMap EntityInstanceMap;
		std::unordered_map<std::string, EntityScript> EntityClassMap;

		std::string assemblyPath;
	};

	AssemblyData data;

	ScriptEngine::ScriptEngine() : Module("ScriptEngine")
	{

	}

	ScriptEngine::~ScriptEngine()
	{

	}
	void ScriptEngine::Init()
	{

	}
	void ScriptEngine::Close()
	{
		MonoShutdown();
	}

	void ScriptEngine::InitAssembly(const char* assemblyPath)
	{
		data.assemblyPath = assemblyPath;
		MonoInit();
		LoadRuntimeAssembly(assemblyPath);
	}
	void ScriptEngine::LoadRuntimeAssembly(const char* path)
	{
		MonoCommand::LoadMonoAssembly(path);
		ScriptRegistry::RegisterAll();
	}
	void ScriptEngine::ReloadRuntimeAssembly()
	{
		ReloadRuntimeAssembly(data.assemblyPath.c_str());
	}
	void ScriptEngine::ReloadRuntimeAssembly(const char* assemblyPath)
	{
		LoadRuntimeAssembly(assemblyPath);
		
		if (data.EntityInstanceMap.size() == 0) return;

		auto& entityMap = data.EntityInstanceMap;
		for (auto& [EntityID, instanceData] : entityMap) {
			ScriptCommand::InitAllEntities();
		}
	}
	bool ScriptEngine::ModuleExists(const std::string& module)
	{
		std::string nameSpace, ClassName;
		ScriptUtils::GetNames(module, nameSpace, ClassName);


		MonoClass* monoClass = MonoCommand::GetMonoClass(nameSpace.c_str(), ClassName.c_str());
		return monoClass != nullptr;
	}
	void ScriptEngine::RegisterScripEntity(const std::string& moduleName, uint32_t id)
	{
		EntityScript& scriptClass = data.EntityClassMap[moduleName];
		scriptClass.moduleName = moduleName;

		ScriptUtils::GetNames(moduleName, scriptClass.nameSpace, scriptClass.className);
		scriptClass.monoClass = MonoCommand::GetMonoClass(scriptClass);

		scriptClass.InitClassMethods();

		EntityInstanceData& entityData = data.EntityInstanceMap[id];
		EntityInstance& instance = entityData.instance;
		instance.ScriptClass = &scriptClass;

		ModuleFieldMap& moduleFieldMap = entityData.moduleFieldMap;
		auto& fieldMap = moduleFieldMap[moduleName];

		std::unordered_map<std::string, PublicField> oldFields;
		oldFields.reserve(fieldMap.size());

		for (auto& [fieldName, field] : fieldMap) {
			oldFields.emplace(fieldName, std::move(field));
		}
		fieldMap.clear();

		MonoClassField* iter;
		void* ptr = 0;

		while ((iter = MonoCommand::GetClassFields(scriptClass.monoClass, &ptr))) {

			const char* name = MonoCommand::GetFieldName(iter);
			if (MonoCommand::GetFieldVisibility(iter) != FieldVisibility::Public) continue;

			VarFieldType fieldType = MonoCommand::GetFieldType(iter);

			if (oldFields.find(name) != oldFields.end()) {
				fieldMap.emplace(name, std::move(oldFields.at(name)));
			}
			else {
				PublicField field = { name, fieldType };
				field.entityInstance = &instance;
				field.monoClassField = iter;
				fieldMap.emplace(name, std::move(field));
			}
		}
	}

	void ScriptEngine::RemoveScriptEntity(const std::string& moduleName, uint32_t id)
	{
		EntityInstanceData& instanceData = GetInstanceData(id);
		ModuleFieldMap& moduleFieldMap = instanceData.moduleFieldMap;

		if (moduleFieldMap.find(moduleName) != moduleFieldMap.end()) {
			moduleFieldMap.erase(moduleName);
		}
	}

	void ScriptEngine::InstantiateScriptEntity(uint32_t handle, const std::string& moduleName)
	{
		EntityInstanceData& data = GetInstanceData(handle);
		EntityInstance& instance = data.instance;
		instance.handle = Instantiate(*instance.ScriptClass);

		void* param[] = { &handle };
		MonoCommand::CallMonoMethod(instance.GetInstance(), instance.ScriptClass->Constructor, param);

		ModuleFieldMap& fieldMap = data.moduleFieldMap;

		if (fieldMap.find(moduleName) != fieldMap.end()) {
			auto& publicFields = fieldMap.at(moduleName);

			for (auto& [name, field] : publicFields) {
				field.CopyStoredToRuntimeValue();
			}
		}
		EntityStart(instance);
	}

	EntityInstanceData& ScriptEngine::GetInstanceData(uint32_t entity)
	{
		return data.EntityInstanceMap[entity];
	}

	void ScriptEngine::Step()
	{
		for (auto& [EntityID, instanceData] : data.EntityInstanceMap) {
			EntityInstance& instance = GetInstanceData(EntityID).instance;
			if (instance.ScriptClass->OnUpdate == nullptr || instance.handle == 0) continue;
			MonoCommand::CallMonoMethod(instance.GetInstance(), instance.ScriptClass->OnUpdate, nullptr);
		}
	}

	void ScriptEngine::EntityStart(EntityInstance& instance)
	{
		if (instance.ScriptClass->OnUpdate)
			MonoCommand::CallMonoMethod(instance.GetInstance(), instance.ScriptClass->OnUpdate);
	}

	void ScriptEngine::MonoInit()
	{
		MonoCommand::InitMono();
		MonoCommand::CreateDomain("Hazard");
	}
	void ScriptEngine::MonoShutdown()
	{
		MonoCommand::Shutdown();
	}

	uint32_t ScriptEngine::Instantiate(EntityScript& instance)
	{
		return MonoCommand::InstantiateHandle(instance);
	}
}