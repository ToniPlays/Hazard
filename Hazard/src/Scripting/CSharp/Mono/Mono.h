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

	enum class FieldVisibility { Private, Protected, Public };

	struct MonoData {
		MonoDomain* mono_domain;

		MonoAssembly* core_assembly;
		MonoImage* core_image;

		MonoAssembly* app_assembly;
		MonoImage* app_image;

		std::string monoCoreAssemblyPath;
	};

	class Mono {
	public:
		static void InitAssembly(ScriptEngineCreateInfo* info);
		static void CreateDomain(const char* name);
		static void LoadRuntimeAssembly(const char* path);
		static void Shutdown();

		static uint32_t InstantiateHandle(MonoClass* monoClass);

		static MonoObject* CallMethod(MonoObject* object, MonoMethod* method, void** params = nullptr);
		static inline MonoObject* TryCallMethod(MonoObject* object, MonoMethod* method, void** params = nullptr) 
		{
			if (method == nullptr) 
				return nullptr;
			return CallMethod(object, method, params);
		}

		static bool ModuleExists(const char* name);

		static MonoObject* ObjectFromHandle(uint32_t handle);
		static void FreeHandle(uint32_t handle);
		static MonoMethod* GetCoreMethod(const std::string& name);
		static MonoMethod* GetAppMethod(const std::string& name);
		static const char* GetMethodFullName(MonoMethod* method);

		//Class functions
		static MonoClass* GetMonoClass(const char* moduleName);
		static MonoClass* GetMonoClass(const char* nameSpace, const char* name);
		static const char* ClassName(MonoClass* monoClass);
		static uint32_t ClassMethodCount(MonoClass* monoClass);
		static MonoMethod* GetClassMethod(MonoClass* monoClass, void** iter);

		static std::vector<MonoClassField*> GetClassFields(MonoClass* monoClass);
		//Field functions
		static void GetFieldValue(MonoObject* object, MonoClassField* field, void* buffer);
		static void SetFieldValue(MonoObject* object, MonoClassField* field, void* buffer);
		static std::string GetFieldName(MonoClassField* field);
		static MonoType* GetFieldType(MonoClassField* field);
		static MonoClassField* GetMonoField(MonoClass* monoClass, void** iter);
		//Type functions
		static int GetType(MonoType* type);
		static char* GetTypeName(MonoType* type);
		static MonoType* ReflectionToType(void* type);
		static MonoType* TypeFromReflectionName(const std::string& name);
		static FieldVisibility GetVisibility(MonoClassField* field);

		static MonoData& GetData() { return s_Data; }
	private:
		static void LoadMonoAssebly(const char* path);
		static MonoAssembly* LoadAssembly(const char* path);
		static MonoImage* GetAssemblyImage(MonoAssembly* assembly);

	private:
		static MonoData s_Data;
	};
}