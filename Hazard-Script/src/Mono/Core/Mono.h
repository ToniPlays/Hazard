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
	enum MonoFlags : uint32_t 
	{
		MonoFlags_Private = BIT(0),
		MonoFlags_Protected = BIT(1),
		MonoFlags_Public = BIT(2),
		MonoFlags_Internal = BIT(3),
		MonoFlags_Static = BIT(4),
		MonoFlags_Abstract = BIT(5),
		MonoFlags_Virtual = BIT(6)
	};

	enum class NativeType {
		None,
		Void,
		Bool,
		Float, Float2, Float3, Float4,
		Double,
		Int8, Int16, Int32, Int64,
		UInt8, UInt16, UInt32, UInt64,
		String,
		Value,
		Reference
	};

	class Mono {
	public:
		static bool Init(const std::string& name);
		static void SetDirs(const std::filesystem::path& assemblyDir, const std::filesystem::path& configDir);
		static void Register(const std::string& signature, void* function);
		static uint32_t InstantiateHandle(MonoClass * monoClass);

		//Mono internal
		static MonoDomain* GetDomain() { return s_Domain; }
		static MonoImage* OpenImage(char* data, uint32_t size, MonoImageOpenStatus& status);
		static MonoAssembly* AssemblyFromImage(MonoImage* image, const std::filesystem::path& path, MonoImageOpenStatus& status);
		static MonoClass* GetMonoClass(MonoImage* image, const std::string& nameSpace, const std::string& name);
		static std::string GetStringProperty(const char* key, MonoClass* monoClass, MonoObject* obj);
		static MonoObject* GetPropertyObject(MonoObject* target, MonoClass* klass, const std::string& name);

		static std::string MonoStringToString(MonoString* string);
		static std::string MonoObjectToString(MonoObject* obj);
		static std::string MonoObjectToChar(MonoObject* obj);
		static MonoString* StringToMonoString(const std::string& string);
		static MonoObject* RuntimeInvoke(MonoObject* target, MonoMethod* method, void** params = nullptr);

		static MonoType* MonoTypeFromReflectionName(const std::string& name, MonoImage* image) {
			return mono_reflection_type_from_name((char*)name.c_str(), image);
		}
		static std::string ResolveClassName(MonoClass* monoClass);

	private:
		inline static MonoDomain* s_Domain = nullptr;
	};
}
