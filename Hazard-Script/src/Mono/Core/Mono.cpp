#include "Mono.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/mono-gc.h>
#include "Core/HazardScriptEngine.h"

#include "Utility/StringUtil.h"

#include "File.h"

namespace HazardScript
{
	bool Mono::Init(const std::string& name)
	{
		MonoDomain* domain = nullptr;
		bool cleanup = false;

		if (s_Domain) 
		{
			domain = mono_domain_create_appdomain((char*)"HazardScriptCore", nullptr);
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
	uint32_t Mono::InstantiateHandle(MonoClass* monoClass)
	{
		MonoObject* obj = mono_object_new(s_Domain, monoClass);
		mono_runtime_object_init(obj);
		return mono_gchandle_new(obj, false);
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
		MonoString* result = (MonoString*)GetPropertyObject(obj, monoClass, key);
		return result != nullptr ? Mono::MonoStringToString(result) : "";
	}

	MonoObject* Mono::GetPropertyObject(MonoObject* target, MonoClass* klass, const std::string& name)
	{
		MonoProperty* prop = mono_class_get_property_from_name(klass, name.c_str());
		MonoMethod* getter = mono_property_get_get_method(prop);
		return RuntimeInvoke(target, getter);
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
			return "";
		}
		return MonoStringToString(mono_object_to_string(obj, nullptr));
	}
	std::string Mono::MonoObjectToChar(MonoObject* obj)
	{
		if (obj == nullptr) {
			return "";
		}
		return MonoStringToString((MonoString*)obj);
	}
	MonoString* Mono::StringToMonoString(const std::string& string) {

		return mono_string_new(s_Domain, string.c_str());
	}

	MonoObject* Mono::RuntimeInvoke(MonoObject* target, MonoMethod* method, void** params)
	{
		MonoObject* exception = nullptr;
		MonoObject* result = mono_runtime_invoke(method, target, params, &exception);

		if (exception)
			HazardScriptEngine::CheckError(exception, result, method);

		return result;
	}

	std::string Mono::ResolveClassName(MonoClass* monoClass)
	{
		const char* className = mono_class_get_name(monoClass);
		std::string name = className != nullptr ? className : "";

		if (name.empty()) return "Unknown";

		MonoClass* nesting = mono_class_get_nesting_type(monoClass);
		if (nesting != nullptr) {
			name = ResolveClassName(nesting) + "/" + name;
		}
		else
		{
			const char* classNameSpace = mono_class_get_namespace(monoClass);
			if (classNameSpace)
				name = std::string(classNameSpace) + "." + name;
		}
		MonoType* classType = mono_class_get_type(monoClass);
		if (mono_type_get_type(classType) == MONO_TYPE_SZARRAY || mono_type_get_type(classType) == MONO_TYPE_ARRAY) {
			name = name.substr(0, StringUtil::OffsetOf(name, '['));
		}
		return name;
	}
}
