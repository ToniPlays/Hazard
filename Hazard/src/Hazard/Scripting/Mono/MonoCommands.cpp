#include "hzrpch.h"
#include "MonoCommands.h"
#include "Hazard/File/File.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>

namespace Hazard::Scripting {

	MonoData MonoCommand::data;


	void MonoCommand::InitMono()
	{
		mono_set_dirs("C:/Program Files/Mono/lib", "C:/Program Files/Mono/etc");
		mono_set_assemblies_path("C:/dev/Hazard//vendor/mono/lib");
	}
	void MonoCommand::CreateDomain(const char* name)
	{
		MonoDomain* domain = mono_jit_init(name);

		std::stringstream ss;
		ss << name << "-runtime";

		char* runtimeName = (char*)ss.str().c_str();
		data.mono_domain = mono_domain_create_appdomain(runtimeName, nullptr);
	}
	void MonoCommand::Shutdown()
	{
		if (data.mono_domain != nullptr) {}
		//mono_jit_cleanup(data.mono_domain);
	}
	uint32_t MonoCommand::InstantiateHandle(EntityScript& script)
	{
		MonoObject* obj = mono_object_new(data.mono_domain, script.monoClass);
		mono_runtime_object_init(obj);
		return mono_gchandle_new(obj, false);
	}
	void MonoCommand::LoadMonoAssembly(const char* path)
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

		if (cleanUp) {
			//mono_domain_unload(data.mono_domain);
			data.mono_domain = domain;
		}

		data.app_assembly = appAssembly;
		data.app_assembly_image = appAssemblyImage;

	}
	void MonoCommand::GetFieldValue(MonoObject* obj, MonoClassField* field, void* value)
	{
		mono_field_get_value(obj, field, value);
	}
	void MonoCommand::SetFieldValue(MonoObject* obj, MonoClassField* field, void* value)
	{
		mono_field_set_value(obj, field, value);
	}
	MonoAssembly* MonoCommand::LoadAssemblyFromFile(const char* file)
	{
		std::vector<char> data = Utility::File::ReadBinaryFile(file);
		MonoImageOpenStatus status;

		MonoImage* image = mono_image_open_from_data_full(data.data(), data.size(), 1, &status, 0);
		MonoAssembly* loaded = mono_assembly_load_from_full(image, file, &status, 0);
		mono_image_close(image);
		return loaded;
	}
	MonoAssembly* MonoCommand::LoadAssembly(const char* file)
	{
		MonoAssembly* assembly = LoadAssemblyFromFile(file);
		return assembly;
	}
	MonoImage* MonoCommand::GetAssemblyImage(MonoAssembly* assembly)
	{
		HZR_CORE_ASSERT((assembly != nullptr), "Cannot read nullptr MonoAssembly image");
		MonoImage* image = mono_assembly_get_image(assembly);
		return image;
	}
	MonoClass* MonoCommand::GetMonoClass(const EntityScript& script) {
		MonoClass* monoClass = mono_class_from_name(data.app_assembly_image, script.nameSpace.c_str(), script.className.c_str());
		return monoClass;
	}
	MonoClass* MonoCommand::GetMonoClass(const char* nameSpace, const char* name)
	{
		return mono_class_from_name(data.app_assembly_image, nameSpace, name);
	}
	MonoObject* MonoCommand::GetClassFromHandle(uint32_t handle)
	{
		return mono_gchandle_get_target(handle);
	}
	MonoClassField* MonoCommand::GetClassFields(MonoClass* monoClass, void** iter)
	{
		return mono_class_get_fields(monoClass, iter);
	}
	const char* MonoCommand::GetFieldName(MonoClassField* field)
	{
		return mono_field_get_name(field);
	}
	VarFieldType MonoCommand::GetFieldType(MonoClassField* field)
	{
		return ScriptUtils::MonoTypeToFieldType(mono_field_get_type(field));
	}
	uint32_t MonoCommand::GetFieldFlags(MonoClassField* field)
	{
		return mono_field_get_flags(field);
	}
	FieldVisibility MonoCommand::GetFieldVisibility(MonoClassField* field)
	{
		uint32_t visibility = mono_field_get_flags(field);
		if ((visibility & MONO_TYPE_ATTR_PUBLIC) == 0) return FieldVisibility::Public;
		return FieldVisibility::Private;
	}
	MonoMethod* MonoCommand::GetMonoAppMethod(const std::string& name) {
		MonoMethodDesc* desc = mono_method_desc_new(name.c_str(), NULL);
		MonoMethod* method = mono_method_desc_search_in_image(desc, data.app_assembly_image);

		return method;
	}
	MonoMethod* MonoCommand::GetMonoCoreMethod(const std::string& name) {
		MonoMethodDesc* desc = mono_method_desc_new(name.c_str(), NULL);
		MonoMethod* method = mono_method_desc_search_in_image(desc, data.core_assembly_image);

		return method;
	}
	MonoObject* MonoCommand::CallMonoMethod(MonoObject* obj, MonoMethod* method, void** params) {
		MonoObject* exception = nullptr;
		MonoObject* result = mono_runtime_invoke(method, obj, params, &exception);
		return result;
	}
}
