#pragma once

#include "UtilityCore.h"
#ifdef HZR_INCLUDE_MONO

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
	class Mono 
	{
	public:
		static bool Init(const std::string& name);
		static void SetDirs(const std::filesystem::path& assemblyDir, const std::filesystem::path& configDir);
		static void Register(const std::string& signature, void* function);
		static uint32_t InstantiateHandle(MonoClass * monoClass);

		//Mono internal
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
		static MonoDomain* GetDomain() { return s_Domain; }

	private:
		inline static MonoDomain* s_Domain = nullptr;
	};
}
#endif
