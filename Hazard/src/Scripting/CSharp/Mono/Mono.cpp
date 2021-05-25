#pragma once
#include <hzrpch.h>
#include "Mono.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>

#include "Hazard/File/File.h"
#include "../ScriptUtils.h"
#include "../ScriptRegistery.h"

namespace Hazard::Scripting::CSharp {

	MonoData Mono::s_Data;

	void Mono::InitAssembly(ScriptEngineCreateInfo* info)
	{
		HZR_PROFILE_FUNCTION();
		s_Data.monoCoreAssemblyPath = info->coreAssemblyPath;
		mono_set_dirs((info->monoDirectory + "/lib").c_str(), (info->monoDirectory + "/etc").c_str());
	}
	void Mono::CreateDomain(const char* name)
	{
		HZR_PROFILE_FUNCTION();
		MonoDomain* domain = mono_jit_init(name);
		std::stringstream ss;
		ss << name << "-runtime";

		s_Data.mono_domain = mono_domain_create_appdomain((char*)ss.str().c_str(), nullptr);
	}
	void Mono::LoadRuntimeAssembly(const char* path)
	{
		if (!File::Exists(path)) {
			HZR_THROW("Runtime assembly does not exist " + std::string(path));
		}
		HZR_PROFILE_FUNCTION();
		LoadMonoAssebly(path);
		ScriptRegistery::Init();
	}
	void Mono::Shutdown()
	{
		if (s_Data.mono_domain != nullptr) {}
	}
	uint32_t Mono::InstantiateHandle(MonoClass* monoClass)
	{
		MonoObject* obj = mono_object_new(s_Data.mono_domain, monoClass);
		mono_runtime_object_init(obj);
		return mono_gchandle_new(obj, false);
	}
	std::vector<MonoClassField*> Mono::GetClassFields(MonoClass* monoClass)
	{
		std::vector<MonoClassField*> result;

		MonoClassField* field;
		void* ptr = 0;

		while ((field = GetMonoField(monoClass, &ptr))) {
			result.push_back(field);
		}
		return result;
	}
	MonoObject* Mono::CallMethod(MonoObject* object, MonoMethod* method, void** params)
	{
		MonoObject* exception = nullptr;
		MonoObject* result = mono_runtime_invoke(method, object, params, &exception);
		//HZR_CORE_ASSERT(exception == nullptr, "[MonoExeption]: {0}", mono_print_unhandled_exception(exception));
		return result;
	}
	bool Mono::ModuleExists(const char* name)
	{
		if (strcmp(name, "") == 0) return false;

		std::string nameSpace, ClassName;
		ScriptUtils::GetNames(name, nameSpace, ClassName);
		return GetMonoClass(nameSpace.c_str(), ClassName.c_str()) != nullptr;
	}
	void Mono::LoadMonoAssebly(const char* path)
	{
		MonoDomain* domain = nullptr;
		bool cleanUp = false;

		if (s_Data.mono_domain) {
			domain = mono_domain_create_appdomain("Hazard-runtime", nullptr);
			mono_domain_set(domain, false);
			cleanUp = true;
		}

		s_Data.core_assembly = LoadAssembly(s_Data.monoCoreAssemblyPath.c_str());
		s_Data.core_image = GetAssemblyImage(s_Data.core_assembly);

		auto appAssembly = LoadAssembly(path);
		auto appAssemblyImage = GetAssemblyImage(appAssembly);

		if (cleanUp) {
			//Something
			s_Data.mono_domain = domain;
		}

		s_Data.app_assembly = appAssembly;
		s_Data.app_image = appAssemblyImage;
	}
	MonoAssembly* Mono::LoadAssembly(const char* path)
	{
		std::vector<char> data = File::ReadBinaryFile(path);
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(data.data(), data.size(), 1, &status, 0);
		MonoAssembly* loaded = mono_assembly_load_from_full(image, path, &status, 0);
		mono_image_close(image);
		return loaded;
	}
	MonoImage* Mono::GetAssemblyImage(MonoAssembly* assembly)
	{
		HZR_CORE_ASSERT((assembly != nullptr), "Cannot read nullptr MonoAssembly image");
		MonoImage* image = mono_assembly_get_image(assembly);
		return image;
	}
	MonoClass* Mono::GetMonoClass(const char* moduleName)
	{
		std::string nameSpace, name;
		ScriptUtils::GetNames(moduleName, nameSpace, name);
		return GetMonoClass(nameSpace.c_str(), name.c_str());
	}
	MonoClass* Mono::GetMonoClass(const char* nameSpace, const char* name)
	{
		return mono_class_from_name(s_Data.app_image, nameSpace, name);
	}
	MonoObject* Mono::ObjectFromHandle(uint32_t handle)
	{
		return mono_gchandle_get_target(handle);
	}
	std::string Mono::GetFieldName(MonoClassField* field)
	{
		return mono_field_get_name(field);
	}
	MonoType* Mono::GetFieldType(MonoClassField* field)
	{
		return mono_field_get_type(field);
	}
	MonoMethod* Mono::GetCoreMethod(const std::string& name)
	{
		MonoMethodDesc* desc = mono_method_desc_new(name.c_str(), NULL);
		MonoMethod* method = mono_method_desc_search_in_image(desc, s_Data.core_image);
		HZR_CORE_ASSERT((method != nullptr), "Method {0} not found", name);
		return method;
	}
	MonoMethod* Mono::GetAppMethod(const std::string& name)
	{
		MonoMethodDesc* desc = mono_method_desc_new(name.c_str(), NULL);
		MonoMethod* method = mono_method_desc_search_in_image(desc, s_Data.app_image);

		return method;
	}
	int Mono::GetType(MonoType* type)
	{
		return mono_type_get_type(type);
	}
	char* Mono::GetTypeName(MonoType* type)
	{
		return mono_type_get_name(type);
	}
	const char* Mono::ClassName(MonoClass* monoClass)
	{
		return mono_class_get_name(monoClass);
	}
	MonoType* Mono::ReflectionToType(void* type)
	{
		return mono_reflection_type_get_type((MonoReflectionType*)type);
	}
	MonoType* Mono::TypeFromReflectionName(const std::string& name)
	{
		return mono_reflection_type_from_name((char*)name.c_str(), s_Data.core_image);
	}
	FieldVisibility Mono::GetVisibility(MonoClassField* field)
	{
		uint32_t flags = mono_field_get_flags(field);
		if (flags & MONO_FIELD_ATTR_PUBLIC) return FieldVisibility::Public;
		if (flags & MONO_FIELD_ATTR_PRIVATE) return FieldVisibility::Protected;
		return FieldVisibility::Protected;
	}
	MonoClassField* Mono::GetMonoField(MonoClass* monoClass, void** iter)
	{
		return mono_class_get_fields(monoClass, iter);
	}
	void Mono::SetFieldValue(MonoObject* object, MonoClassField* field, void* buffer)
	{
		mono_field_set_value(object, field, buffer);
	}
	void Mono::GetFieldValue(MonoObject* object, MonoClassField* field, void* buffer)
	{
		mono_field_get_value(object, field, buffer);
	}
}
