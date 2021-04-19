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

	MonoData Mono::data;

	void Mono::InitAssembly(const char* path)
	{
		HZR_CORE_INFO("Initializing mono from " + std::string(path));

		mono_set_dirs("C:/Program Files/Mono/lib", "C:/Program Files/Mono/etc");
		mono_set_assemblies_path("C:/dev/Hazard//vendor/mono/lib");
	}
	void Mono::CreateDomain(const char* name)
	{
		MonoDomain* domain = mono_jit_init(name);
		std::stringstream ss;
		ss << name << "-runtime";

		data.mono_domain = mono_domain_create_appdomain((char*)ss.str().c_str(), nullptr);
	}
	void Mono::LoadRuntimeAssembly(const char* path)
	{
		HZR_CORE_INFO("Mono load runtime assembly");
		LoadMonoAssebly(path);

		ScriptRegistery::Init();
	}
	uint32_t Mono::InstantiateHandle(MonoClass* monoClass)
	{
		MonoObject* obj = mono_object_new(data.mono_domain, monoClass);
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
	MonoObject* Mono::CallMethod(MonoClass* monoClass, MonoMethod* method, void** params)
	{
		MonoObject* exception = nullptr;
		MonoObject* result = mono_runtime_invoke(method, monoClass, params, &exception);
		HZR_CORE_ASSERT((exception == nullptr), "Mono method failed {0}: {1}", mono_method_get_name(method));
		return result;
	}
	bool Mono::ModuleExists(const char* name)
	{
		std::string nameSpace, ClassName;
		ScriptUtils::GetNames(name, nameSpace, ClassName);
		return GetMonoClass(nameSpace.c_str(), ClassName.c_str());
	}
	void Mono::LoadMonoAssebly(const char* path)
	{
		MonoDomain* domain = nullptr;
		bool cleanUp = false;

		if (data.mono_domain) {
			domain = mono_domain_create_appdomain("Hazard-runtime", nullptr);
			mono_domain_set(domain, false);
			cleanUp = true;
		}
		data.core_assembly = LoadAssembly("c:/dev/Hazard/HazardScripting/bin/debug/netstandard2.0/HazardScripting.dll");
		data.core_image = GetAssemblyImage(data.core_assembly);

		auto appAssembly = LoadAssembly(path);
		auto appAssemblyImage = GetAssemblyImage(appAssembly);

		if (cleanUp) {
			//Something
			data.mono_domain = domain;
		}

		data.app_assembly = appAssembly;
		data.app_image = appAssemblyImage;
	}
	MonoAssembly* Mono::LoadAssembly(const char* path)
	{
		std::vector<char> data = Utility::File::ReadBinaryFile(path);
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
		return mono_class_from_name(data.app_image, nameSpace, name);
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
		MonoMethod* method = mono_method_desc_search_in_image(desc, data.core_image);
		HZR_CORE_ASSERT((method != nullptr), "Method {0} not found", name);
		return method;
	}
	MonoMethod* Mono::GetAppMethod(const std::string& name)
	{
		MonoMethodDesc* desc = mono_method_desc_new(name.c_str(), NULL);
		MonoMethod* method = mono_method_desc_search_in_image(desc, data.app_image);

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
	MonoType* Mono::ReflectionToType(void* type)
	{
		return mono_reflection_type_get_type((MonoReflectionType*)type);
	}
	MonoType* Mono::TypeFromReflectionName(const std::string& name)
	{
		return mono_reflection_type_from_name((char*)name.c_str(), data.core_image);
	}
	MonoClassField* Mono::GetMonoField(MonoClass* monoClass, void** iter)
	{
		return mono_class_get_fields(monoClass, iter);
	}
}
