#pragma once

#include <hzrpch.h>
#include "ScriptEngine.h"
#include "Hazard/File/File.h"

#include "ScriptUtils.h"
#include "ScriptCommand.h"
#include "Script/ScriptRegistry.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>


namespace Hazard::Scripting {
#pragma region MONO_DATA 
	struct MonoData {
		MonoDomain* mono_domain;
		MonoAssembly* core_assembly;
		MonoImage* core_assembly_image;

		MonoAssembly* app_assembly;
		MonoImage* app_assembly_image;

		EntityInstanceMap EntityInstanceMap;
		std::unordered_map<std::string, EntityScript> EntityClassMap;

		std::string assemblyPath;
	};

	MonoAssembly* LoadAssemblyFromFile(const char* file)
	{
		std::vector<char> data = File::File::ReadBinaryFile(file);
		MonoImageOpenStatus status;

		MonoImage* image = mono_image_open_from_data_full(data.data(), data.size(), 1, &status, 0);
		MonoAssembly* loaded = mono_assembly_load_from_full(image, file, &status, 0);
		mono_image_close(image);
		return loaded;
	}

	MonoAssembly* LoadAssembly(const char* file)
	{
		MonoAssembly* assembly = LoadAssemblyFromFile(file);
		return assembly;
	}
	MonoImage* GetAssemblyImage(MonoAssembly* assembly)
	{
		HZR_CORE_ASSERT((assembly != nullptr), "Cannot read nullptr MonoAssembly image");
		MonoImage* image = mono_assembly_get_image(assembly);
		return image;
	}

	MonoClass* GetMonoClass(MonoImage* image, const EntityScript& script) {
		MonoClass* monoClass = mono_class_from_name(image, script.nameSpace.c_str(), script.className.c_str());
		return monoClass;
	}

	MonoMethod* GetMonoMethod(MonoImage* image, const std::string& name) {
		MonoMethodDesc* desc = mono_method_desc_new(name.c_str(), NULL);
		MonoMethod* method = mono_method_desc_search_in_image(desc, image);

		if (method == nullptr) HZR_CORE_ERROR("Unable to find {0}", name);

		return method;
	}
	MonoObject* CallMonoMethod(MonoObject* obj, MonoMethod* method, void** params = nullptr) {
		MonoObject* exception = nullptr;
		MonoObject* result = mono_runtime_invoke(method, obj, params, &exception);
		return result;
	}

#pragma endregion

	static VarFieldType MonoTypeToFieldType(MonoType* type)
	{
		int t = mono_type_get_type(type);

		HZR_CORE_INFO(t);
		switch (t)
		{
		case MONO_TYPE_R4:			return VarFieldType::Float;
		case MONO_TYPE_I4:			return VarFieldType::Int;
		case MONO_TYPE_U4:			return VarFieldType::UnsignedInt;
		case MONO_TYPE_STRING:		return VarFieldType::String;
		case MONO_TYPE_CLASS:
		{
			char* name = mono_type_get_name(type);
			if (strcmp(name, "Hazard.Vector2") == 0) return VarFieldType::Vec2;
			if (strcmp(name, "Hazard.Vector3") == 0) return VarFieldType::Vec3;
			if (strcmp(name, "Hazard.Vector4") == 0) return VarFieldType::Vec4;
		}
		}
		return VarFieldType::None;
	}

	MonoData data;

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
		MonoDomain* domain = nullptr;
		bool cleanUp = false;

		if (data.mono_domain)
		{

			domain = mono_domain_create_appdomain("Hazard-runtime", nullptr);
			mono_domain_set(domain, false);
			cleanUp = true;
		}

		data.core_assembly = LoadAssembly("c:/dev/Hazard/HazardScripting/bin/debug/netstandard2.0/HazardScripting.dll");
		data.core_assembly_image = GetAssemblyImage(data.core_assembly);

		auto appAssembly = LoadAssembly(path);
		auto appAssemblyImage = GetAssemblyImage(appAssembly);

		ScriptRegistry::RegisterAll();

		if (cleanUp) {
			//mono_domain_unload(data.mono_domain);
			data.mono_domain = domain;
		}

		data.app_assembly = appAssembly;
		data.app_assembly_image = appAssemblyImage;

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

		MonoClass* monoClass = mono_class_from_name(data.app_assembly_image, nameSpace.c_str(), ClassName.c_str());
		return monoClass != nullptr;
	}
	void ScriptEngine::RegisterScripEntity(const std::string& moduleName, uint32_t id)
	{
		EntityScript& scriptClass = data.EntityClassMap[moduleName];
		scriptClass.moduleName = moduleName;

		ScriptUtils::GetNames(moduleName, scriptClass.nameSpace, scriptClass.className);
		scriptClass.monoClass = GetMonoClass(data.app_assembly_image, scriptClass);

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

		while ((iter = mono_class_get_fields(scriptClass.monoClass, &ptr))) {

			const char* name = mono_field_get_name(iter);
			uint32_t flags = mono_field_get_flags(iter);

			if ((flags & MONO_FIELD_ATTR_PUBLIC) == 0) continue;

			VarFieldType fieldType = MonoTypeToFieldType(mono_field_get_type(iter));

			MonoCustomAttrInfo* atff = mono_custom_attrs_from_field(scriptClass.monoClass, iter);
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
		CallMonoMethod(instance.GetInstance(), instance.ScriptClass->Constructor, param);

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

			HZR_CORE_INFO("Script step");
			CallMonoMethod(instance.GetInstance(), instance.ScriptClass->OnUpdate, nullptr);
		}
	}

	void ScriptEngine::EntityStart(EntityInstance& instance)
	{
		if (instance.ScriptClass->OnUpdate)
			CallMonoMethod(instance.GetInstance(), instance.ScriptClass->OnUpdate);
	}

	void ScriptEngine::MonoInit()
	{
		ScriptUtils::InitMono();
		MonoDomain* domain = mono_jit_init("Hazard");

		char* name = (char*)"Hazard-runtime";
		data.mono_domain = mono_domain_create_appdomain(name, nullptr);
	}
	void ScriptEngine::MonoShutdown()
	{
		if (data.mono_domain != nullptr) {}
		//mono_jit_cleanup(data.mono_domain);
	}

	uint32_t ScriptEngine::Instantiate(EntityScript& instance)
	{
		MonoObject* obj = mono_object_new(data.mono_domain, instance.monoClass);

		mono_runtime_object_init(obj);
		return mono_gchandle_new(obj, false);
	}

	MonoObject* EntityInstance::GetInstance()
	{
		HZR_ASSERT(handle != 0, "Invalid entity instance");
		return mono_gchandle_get_target(handle);
	}

	void PublicField::CopyStoredToRuntimeValue()
	{
		mono_field_set_value(entityInstance->GetInstance(), monoClassField, storedValueBuffer);
	}

	bool PublicField::RuntimeAvailable()
	{
		return entityInstance->handle != 0;
	}

	void PublicField::SetStoredValueRaw(void* src)
	{
		uint32_t size = ScriptUtils::GetFieldSize(type);
		memcpy(storedValueBuffer, src, size);
	}

	std::byte* PublicField::AllocateBuffer(VarFieldType type)
	{
		uint32_t size = ScriptUtils::GetFieldSize(type);
		std::byte* buffer = new std::byte[size];
		memset(buffer, 0, size);
		return buffer;
	}

	void PublicField::SetStoredValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(type);
		memcpy(storedValueBuffer, value, size);
	}

	void PublicField::GetStoredValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(type);
		memcpy(value, storedValueBuffer, size);
	}

	void PublicField::SetRuntimeValueInternal(void* value) const
	{
		mono_field_set_value(entityInstance->GetInstance(), monoClassField, value);
	}

	void PublicField::GetRuntimeValueInternal(void* value) const
	{
		mono_field_get_value(entityInstance->GetInstance(), monoClassField, value);
	}
	void EntityScript::InitClassMethods()
	{
		Constructor = GetMonoMethod(data.core_assembly_image, "Hazard.Entity:.ctor(ulong)");
		OnUpdate = GetMonoMethod(data.app_assembly_image, moduleName + ":OnUpdate()");
		
	}
}