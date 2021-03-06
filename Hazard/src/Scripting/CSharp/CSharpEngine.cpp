#pragma once

#include <hzrpch.h>
#include "CSharpEngine.h"
#include "Mono/Mono.h"
#include "CSharpField.h"
#include "ScriptUtils.h"
#include "Hazard/Scripting/ScriptCommand.h"

extern "C"
{
	typedef struct _MonoObject MonoObject;
}

namespace Hazard::Scripting::CSharp {

	struct AssemblyData {
		EntityInstanceMap entityInstanceMap;
		std::unordered_map<std::string, EntityScript> entityClassMap;

		std::string assemblyPath;
	};

	AssemblyData data;

	CSharpEngine::CSharpEngine(ScriptEngineCreateInfo* info)
	{
		HZR_PROFILE_FUNCTION();
		Mono::InitAssembly(info);
		Mono::CreateDomain("Hazard");
		Mono::LoadRuntimeAssembly(info->appAssemblyPath.c_str());
	}
	void CSharpEngine::OnBeginRuntime()
	{

	}
	void CSharpEngine::OnEndRuntime()
	{
	
	}
	bool CSharpEngine::ModuleExists(const char* name)
	{
		MonoClass* monoClass = Mono::GetMonoClass(name);
		return monoClass != nullptr;
	}
	void CSharpEngine::UpdateEntities()
	{
		float delta = Time::s_DeltaTime;
		void* param[] = { &delta };

		for (auto& [id, entity] : data.entityInstanceMap) {
			EntityInstance& instance = entity.instance;

			Mono::TryCallMethod(instance.GetInstance(), instance.ScriptClass->OnUpdate, param);
		}
	}
	void CSharpEngine::OnSceneLoaded()
	{
	}
	void CSharpEngine::OnSceneUnloaded()
	{
	}
	std::unordered_map<std::string, PublicField*> CSharpEngine::GetPublicFields(uint32_t entity, const std::string& moduleName)
	{
		ModuleFieldMap& data = GetInstanceData(entity).moduleFieldMap;
		std::unordered_map<std::string, CSharpField*> fields = data.at(moduleName);

		std::unordered_map<std::string, PublicField*> result;
		for (auto [name, f] : fields) {
			result.emplace(name, f);
		}
		return result;
	}
	void CSharpEngine::InitializeEntity(uint32_t entity, const std::string& moduleName)
	{
		if (!ModuleExists(moduleName.c_str())) {
			HZR_CORE_ERROR("[Mono] Module does not exist {0}", moduleName);
			return;
		}
		EntityScript& scriptClass = data.entityClassMap[moduleName];
		scriptClass.moduleName = moduleName;

		ScriptUtils::GetNames(moduleName, scriptClass.nameSpace, scriptClass.className);
		scriptClass.monoClass = Mono::GetMonoClass(scriptClass.className.c_str());

		scriptClass.InitClassMethods();

		EntityInstanceData& entityData = data.entityInstanceMap[entity];
		EntityInstance& instance = entityData.instance;
		instance.ScriptClass = &scriptClass;

		ModuleFieldMap& moduleFieldMap = entityData.moduleFieldMap;
		auto& fieldMap = moduleFieldMap[moduleName];

		std::unordered_map<std::string, CSharpField*> oldFields;
		oldFields.reserve(fieldMap.size());

		for (auto& [fieldName, field] : fieldMap) {
			oldFields.emplace(fieldName, std::move(field));
		}
		fieldMap.clear();

		MonoClassField* iter;
		void* ptr = 0;

		instance.handle = Mono::InstantiateHandle(instance.ScriptClass->monoClass);

		while ((iter = Mono::GetMonoField(scriptClass.monoClass, &ptr))) {

			std::string name = Mono::GetFieldName(iter);
			if (Mono::GetVisibility(iter) != FieldVisibility::Public) continue;

			FieldType fieldType = ScriptUtils::GetFieldType(iter);

			if (oldFields.find(name) != oldFields.end()) {
				fieldMap.emplace(name, std::move(oldFields.at(name)));
			}
			else {
				CSharpField* field = new CSharpField(fieldType);
				field->SetEntityInstance(&instance);
				field->SetField(iter);
				float val;
				Mono::GetFieldValue(Mono::ObjectFromHandle(instance.handle), iter, &val);
				field->SetStoredValue<float>(val);
				fieldMap.emplace(name, std::move(field));
			}
		}
	}
	void CSharpEngine::Instantiate(uint32_t entity, const std::string& moduleName)
	{
		EntityInstanceData& data = GetInstanceData(entity);
		EntityInstance& instance = data.instance;

		void* param[] = { &entity };
		Mono::CallMethod(instance.GetInstance(), instance.ScriptClass->Constructor, param);

		ModuleFieldMap& fieldMap = data.moduleFieldMap;

		if (fieldMap.find(moduleName) != fieldMap.end()) {
			auto& publicFields = fieldMap.at(moduleName);

			for (auto& [name, field] : publicFields) {
				field->CopyStoredToRuntimeValue();
			}
		}
		Mono::TryCallMethod(instance.GetInstance(), instance.ScriptClass->OnCreated);
	}
	void CSharpEngine::ClearEntity(uint32_t entity, const std::string& moduleName)
	{
		EntityInstanceData& instanceData = GetInstanceData(entity);
		ModuleFieldMap& moduleFieldMap = instanceData.moduleFieldMap;

		if (moduleFieldMap.find(moduleName) != moduleFieldMap.end()) {
			moduleFieldMap.erase(moduleName);
		}
	}
	void CSharpEngine::OnCreate(uint32_t entity)
	{

	}
	void CSharpEngine::OnStart(uint32_t entity)
	{

	}
	void CSharpEngine::OnUpdate(uint32_t entity)
	{
	}
	void CSharpEngine::OnLateUpdate(uint32_t entity)
	{
	}
	void CSharpEngine::OnFixedUpdate(uint32_t entity)
	{
	}
	void CSharpEngine::OnEnable(uint32_t entity)
	{
	}
	void CSharpEngine::OnDisable(uint32_t entity)
	{
	}
	void CSharpEngine::OnDestroy(uint32_t entity)
	{
	}
	void CSharpEngine::OnCollision(uint32_t entity)
	{
	}
	EntityInstanceData& CSharpEngine::GetInstanceData(uint32_t entity)
	{
		return data.entityInstanceMap[entity];
	}
	void CSharpEngine::OnApplicationClose()
	{
		Mono::Shutdown();
	}
	void CSharpEngine::Reload()
	{
		Mono::LoadRuntimeAssembly("c:/dev/HazardProject/bin/Debug/netstandard2.0/HazardProject.dll");

		if (data.entityInstanceMap.size() == 0) return;
		auto& entityMap = data.entityInstanceMap;

		for (auto& [EntityID, instanceData] : entityMap) {
			ScriptCommand::InitAllEntities();
		}
	}
}