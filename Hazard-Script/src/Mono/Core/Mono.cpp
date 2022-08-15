#include "Mono.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/mono-gc.h>

#include <mono/metadata/tabledefs.h>

#include "File.h"
#include "Utility/StringUtil.h"


#include "Mono/Core/Mono.h"


#define CACHED_CLASS_NAME "Cached"

namespace HazardScript
{
	bool Mono::Init(const std::string& name)
	{
		MonoDomain* domain = nullptr;
		bool cleanup = false;

		if (s_Domain) {
			domain = mono_domain_create_appdomain("HazardScriptCore", nullptr);
			mono_domain_set(domain, false);
			cleanup = true;
		}
		else
		{
			domain = mono_jit_init(name.c_str());

		}
		s_Domain = mono_domain_create_appdomain((char*)name.c_str(), nullptr);

		return s_Domain != nullptr;
	}
	void Mono::SetDirs(const std::filesystem::path& assemblyDir, const std::filesystem::path& configDir)
	{
		mono_set_dirs(assemblyDir.string().c_str(), configDir.string().c_str());
	}
	void Mono::Register(const std::string& signature, void* function)
	{
		mono_add_internal_call(signature.c_str(), function);
	}
	FieldVisibility Mono::GetMethodVisibility(MonoMethod* method)
	{
		uint32_t iFlags = 0;
		uint32_t flags = mono_method_get_flags(method, &iFlags);

		if (flags & MONO_FIELD_ATTR_PUBLIC)
			return FieldVisibility::Public;
		if (flags & MONO_FIELD_ATTR_PRIVATE)
			return FieldVisibility::Private;

		return FieldVisibility::Protected;
	}
	uint32_t Mono::InstantiateHandle(MonoClass* monoClass)
	{
		MonoObject* obj = mono_object_new(s_Domain, monoClass);
		mono_runtime_object_init(obj);
		return mono_gchandle_new(obj, false);
	}

	void Mono::GetFieldValue(MonoObject* object, MonoClassField* field, void* buffer)
	{
		mono_field_get_value(object, field, buffer);
	}
	void Mono::SetFieldValue(MonoObject* object, MonoClassField* field, void* buffer)
	{
		mono_field_set_value(object, field, buffer);
	}

	MonoImage* Mono::OpenImage(char* data, uint32_t size, MonoImageOpenStatus& status)
	{
		return mono_image_open_from_data_full((char*)data, (uint32_t)size, 1, &status, 0);
	}
	MonoAssembly* Mono::AssemblyFromImage(MonoImage* image, const std::filesystem::path& path, MonoImageOpenStatus& status)
	{
		HZR_ASSERT(File::Exists(path), "Assembly file does not exist: " + path.string());
		return mono_assembly_load_from_full(image, path.string().c_str(), &status, 0);
	}
	MonoClass* Mono::GetMonoClass(MonoImage* image, const std::string& nameSpace, const std::string& name)
	{
		return mono_class_from_name(image, nameSpace.c_str(), name.c_str());
	}
	std::string Mono::GetStringProperty(const char* key, MonoClass* monoClass, MonoObject* obj)
	{
		MonoProperty* property = mono_class_get_property_from_name(monoClass, key);
		MonoMethod* getter = mono_property_get_get_method(property);
		MonoString* result = (MonoString*)mono_runtime_invoke(getter, obj, NULL, NULL);
		return result != nullptr ? Mono::MonoStringToString(result) : "";
	}
	std::string Mono::MonoStringToString(MonoString* string)
	{
		MonoError error;
		char* ptr = mono_string_to_utf8_checked(string, &error);
		if (!mono_error_ok(&error)) {
			unsigned short errorCode = mono_error_get_error_code(&error);
			const char* errorMessage = mono_error_get_message(&error);
			printf("Mono Error!\n");
			printf("\tError Code: %hu\n", errorCode);
			printf("\tError Message: %s\n", errorMessage);
			mono_error_cleanup(&error);
		}
		if (!ptr) return "";
		std::string result(ptr);
		mono_free(ptr);
		return result;
	}
	std::string Mono::MonoObjectToString(MonoObject* obj)
	{
		if (obj == nullptr) {
			return "NULL";
		}
		std::string b = MonoStringToString((MonoString*)obj);
		return b;
	}
	std::string Mono::MonoObjectToChar(MonoObject* obj)
	{
		if (obj == nullptr) {
			return "NULL";
		}
		MonoString* string = mono_object_to_string(obj, nullptr);
		std::string b = MonoStringToString(string);
		return b;
	}
	MonoString* Mono::StringToMonoString(const std::string& string) {

		const char* str = string.c_str();
		return mono_string_new(s_Domain, str);
	}
	FieldVisibility Mono::GetFieldVisibility(MonoClassField* field)
	{
		uint32_t flags = mono_field_get_flags(field);

		if (flags & MONO_FIELD_ATTR_PUBLIC)
			return FieldVisibility::Public;
		if (flags & MONO_FIELD_ATTR_PRIVATE)
			return FieldVisibility::Private;

		return FieldVisibility::Protected;
	}
	FieldType Mono::GetFieldType(MonoClassField* field)
	{
		MonoType* type = mono_field_get_type(field);

		switch (mono_type_get_type(type))
		{
		case MONO_TYPE_R4:			return FieldType::Float;
		case MONO_TYPE_I4:			return FieldType::Int;
		case MONO_TYPE_U4:			return FieldType::UInt;
		case MONO_TYPE_STRING:		return FieldType::String;
		case MONO_TYPE_VALUETYPE:
		{

			char* name = mono_type_get_name(type);
			if (strcmp(name, "Hazard.Vector2") == 0) return FieldType::Float2;
			if (strcmp(name, "Hazard.Vector3") == 0) return FieldType::Float3;
			if (strcmp(name, "Hazard.Vector4") == 0) return FieldType::Float4;
		}
		}
		return FieldType::None;
	}
}
