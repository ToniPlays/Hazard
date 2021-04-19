#pragma once

#include "Hazard/Core/Core.h"

extern "C"
{
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoType MonoType;
	typedef struct _MonoMethod MonoMethod;

	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
}

namespace Hazard::Scripting::CSharp {

	struct MonoData {
		MonoDomain* mono_domain;

		MonoAssembly* core_assembly;
		MonoImage* core_image;

		MonoAssembly* app_assembly;
		MonoImage* app_image;
	};

	class Mono {
	public:
		static void InitAssembly(const char* path);
		static void CreateDomain(const char* name);
		static void LoadRuntimeAssembly(const char* path);
		static uint32_t InstantiateHandle(MonoClass* monoClass);

		static std::vector<MonoClassField*> GetClassFields(MonoClass* monoClass);

		static MonoObject* CallMethod(MonoClass* monoClass, MonoMethod* method, void** params = nullptr);

		static bool ModuleExists(const char* name);
		static MonoData& GetData() { return data; }
		static MonoClass* GetMonoClass(const char* moduleName);
		static MonoClass* GetMonoClass(const char* nameSpace, const char* name);
		static std::string GetFieldName(MonoClassField* field);
		static MonoType* GetFieldType(MonoClassField* field);
		static MonoMethod* GetCoreMethod(const std::string& name);
		static MonoMethod* GetAppMethod(const std::string& name);
		static int GetType(MonoType* type);
		static char* GetTypeName(MonoType* type);
		static MonoType* ReflectionToType(void* type);
		static MonoType* TypeFromReflectionName(const std::string& name);

	private:
		static void LoadMonoAssebly(const char* path);
		static MonoAssembly* LoadAssembly(const char* path);
		static MonoImage* GetAssemblyImage(MonoAssembly* assembly);

		static MonoClassField* GetMonoField(MonoClass* monoClass, void** iter);
	private:

		static MonoData data;
	};
}