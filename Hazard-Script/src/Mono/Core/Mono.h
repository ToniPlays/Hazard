#pragma once

#include "UtilityCore.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/mono-gc.h>

//#define REGISTER_INTERNAL(x) Mono::Register("Hazard.InternalCalls::#x", (void*)#x);

namespace HazardScript
{
	enum class FieldVisibility { Private, Protected, Public };
	enum class FieldType { None, Float, Float2, Float3, Float4, Int, UInt, String, ManagedType };

	class Mono {
	public:
		static bool Init(const std::string& name);
		static void SetDirs(const std::filesystem::path& assemblyDir, const std::filesystem::path& configDir);
		static void Register(const std::string& signature, void* function);
		static FieldVisibility GetMethodVisibility(MonoMethod* method);
		static uint32_t InstantiateHandle(MonoClass* monoClass);
		template<typename T>
		static T GetFieldValue(MonoObject* object, MonoClassField* field) {
			T value;
			GetFieldValue(object, field, &value);
			return value;
		}

		template<typename T>
		static T GetFieldValue(MonoObject* object, const std::string& name) {
			MonoClass* monoClass = mono_object_get_class(object);
			return Mono::GetFieldValue<T>(object, mono_class_get_field_from_name(monoClass, name.c_str()));
		}
		template<>
		static std::string GetFieldValue(MonoObject* object, MonoClassField* field) {
			MonoString* value;
			GetFieldValue(object, field, &value);
			return MonoStringToString(value);
		}

		static void GetFieldValue(MonoObject* object, MonoClassField* field, void* buffer);
		static void SetFieldValue(MonoObject* object, MonoClassField* field, void* buffer);

		//Mono internal
		static const MonoDomain* GetDomain() { return s_Domain; }
		static MonoImage* OpenImage(char* data, uint32_t size, MonoImageOpenStatus& status);
		static MonoAssembly* AssemblyFromImage(MonoImage* image, const std::filesystem::path& path, MonoImageOpenStatus& status);
		static MonoClass* GetMonoClass(MonoImage* image, const std::string& nameSpace, const std::string& name);
		static std::string GetStringProperty(const char* key, MonoClass* monoClass, MonoObject* obj);

		static std::string MonoStringToString(MonoString* string);
		static std::string MonoObjectToString(MonoObject* obj);
		static FieldVisibility GetFieldVisibility(MonoClassField* field);

	private:
		inline static MonoDomain* s_Domain = nullptr;
	};
}