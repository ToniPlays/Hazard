#pragma once

#include "../PublicField.h"

extern "C"
{
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoMethod MonoMethod;

	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
}

namespace Hazard::Scripting {

	struct MonoData {
		MonoDomain* mono_domain;
		MonoAssembly* core_assembly;
		MonoImage* core_assembly_image;

		MonoAssembly* app_assembly;
		MonoImage* app_assembly_image;
	};

	enum class FieldVisibility {
		Public,
		Private,
		Protected
	};

	class MonoCommand {
	public:
		static void InitMono();
		static void CreateDomain(const char* name);
		static void Shutdown();

		static uint32_t InstantiateHandle(EntityScript& script);

		static void LoadMonoAssembly(const char* path);

		static void GetFieldValue(MonoObject* obj, MonoClassField* field, void* value);
		static void SetFieldValue(MonoObject* obj, MonoClassField* field, void* value);

		static MonoAssembly* LoadAssemblyFromFile(const char* file);
		static MonoAssembly* LoadAssembly(const char* file);
		static MonoImage* GetAssemblyImage(MonoAssembly* assembly);
		static MonoClass* GetMonoClass(const EntityScript& script);
		static MonoClass* GetMonoClass(const char* nameSpace, const char* name);
		static MonoObject* GetClassFromHandle(uint32_t handle);

		static MonoClassField* GetClassFields(MonoClass* monoClass, void** iter);
		static const char* GetFieldName(MonoClassField* field);
		static VarFieldType GetFieldType(MonoClassField* field);
		static uint32_t GetFieldFlags(MonoClassField* field);
		static FieldVisibility GetFieldVisibility(MonoClassField* field);

		static MonoMethod* GetMonoAppMethod(const std::string& name);
		static MonoMethod* GetMonoCoreMethod(const std::string& name);
		static MonoObject* CallMonoMethod(MonoObject* obj, MonoMethod* method, void** params = nullptr);

		static MonoData& GetData() { return data; }

	private:
		static MonoData data;
	};
}