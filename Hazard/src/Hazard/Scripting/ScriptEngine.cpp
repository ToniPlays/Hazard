#pragma once

#include <hzrpch.h>
#include "ScriptEngine.h"
#include "Hazard/File/File.h"

#include "ScriptUtils.h"

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

	MonoClass* GetClass(MonoImage* image, const EntityScript& script) {
		MonoClass* monoClass = mono_class_from_name(image, script.nameSpace.c_str(), script.className.c_str());
		return monoClass;
	}
#pragma endregion

	static VarFieldType MonoTypeToFieldType(MonoType* type)
	{
		int t = mono_type_get_type(type);

		switch (t)
		{
		case MONO_TYPE_R4:			return VarFieldType::Float;
		case MONO_TYPE_I4:			return VarFieldType::Int;
		case MONO_TYPE_U4:			return VarFieldType::UnsignedInt;
		case MONO_TYPE_STRING:		return VarFieldType::String;
		case MONO_TYPE_VALUETYPE:
		{
			char* name = mono_type_get_name(type);

			if (strcmp(name, "Hazard.Vector2") == 0) return VarFieldType::Vec2;
			if (strcmp(name, "Hazard.Vector3") == 0) return VarFieldType::Vec3;
			if (strcmp(name, "Hazard.Vector4") == 0) return VarFieldType::Vec4;
		}
		return VarFieldType::None;
		}
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
		HZR_CORE_INFO("AssemblyPath {0}", assemblyPath);
		data.assemblyPath = assemblyPath;
		MonoInit();
		LoadRuntimeAssembly(assemblyPath);
	}
	void ScriptEngine::LoadRuntimeAssembly(const char* path)
	{
		MonoDomain* domain = nullptr;
		bool cleanUp = false;

		if (data.mono_domain != nullptr)
		{
			domain = mono_domain_create_appdomain("Hazard-runtime", nullptr);
			mono_domain_set(domain, false);
			cleanUp = true;
		}
		HZR_CORE_WARN("Loading runtime assembly");
		data.core_assembly = LoadAssembly("res/ScriptEngine.dll");

		data.core_assembly_image = GetAssemblyImage(data.core_assembly);

		auto appAssembly = LoadAssembly(path);
		auto appAssemblyImage = GetAssemblyImage(appAssembly);

		if (cleanUp) {
			mono_domain_unload(data.mono_domain);
			data.mono_domain = domain;
		}

		data.app_assembly = appAssembly;
		data.app_assembly_image = appAssemblyImage;

		HZR_CORE_INFO("Loaded successfully");
	}
	void ScriptEngine::ReloadAssembly()
	{
		ReloadAssembly(data.assemblyPath.c_str());
	}
	void ScriptEngine::ReloadAssembly(const char* assemblyPath)
	{
		LoadRuntimeAssembly(assemblyPath);
		HZR_CORE_INFO("Reloading C# assembly");
	}
	bool ScriptEngine::ModuleExists(const std::string& module)
	{
		std::string nameSpace, ClassName;
		ScriptUtils::GetNames(module, nameSpace, ClassName);

		MonoClass* monoClass = mono_class_from_name(data.app_assembly_image, nameSpace.c_str(), ClassName.c_str());
		HZR_CORE_INFO("{0}::{1} exists {2}", nameSpace, ClassName, monoClass != nullptr);
		return monoClass != nullptr;
	}

	void ScriptEngine::RegisterScriptableEntity(const std::string& moduleName, uint32_t id)
	{
		EntityScript& scriptClass = data.EntityClassMap[moduleName];
		scriptClass.moduleName = moduleName;

		ScriptUtils::GetNames(moduleName, scriptClass.nameSpace, scriptClass.className);
		scriptClass.monoClass = GetClass(data.app_assembly_image, scriptClass);

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
			if (oldFields.find(name) != oldFields.end())
				fieldMap.emplace(name, std::move(oldFields.at(name)));
			else {
				PublicField field = { name, fieldType };
				field.entityInstance = &instance;
				field.monoClassField = iter;
				fieldMap.emplace(name, std::move(field));
			}
		}
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

	MonoObject* EntityInstance::GetInstance()
	{
		return mono_gchandle_get_target(handle);
	}

}