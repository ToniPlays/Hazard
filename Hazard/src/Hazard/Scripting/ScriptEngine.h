#pragma once

#include "Hazard/Module/Module.h"
#include "ScriptUtils.h"
#include "Hazard/Entity/Scene.h"


extern "C"
{
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoClass MonoClass;
}


namespace Hazard::Scripting {


#pragma region PUBLIC_FIELD

	struct EntityScript;

	struct EntityInstance
	{
		EntityScript* ScriptClass;
		uint32_t handle;
		ECS::Scene* instance = nullptr;
		MonoObject* GetInstance();
	};
	struct PublicField {
		std::string name;
		VarFieldType type;

		PublicField() = default;
		PublicField(const std::string& name, VarFieldType type) : name(name), type(type) {
			storedValueBuffer = AllocateBuffer(type);
		};

		~PublicField() {
			//delete[] storedValueBuffer;
		};

		void CopyStoredToRuntimeValue();
		bool RuntimeAvailable();

		template<typename T>
		T GetStoredValue() const {
			T value;
			GetStoredValueInternal(&value);
			return value;
		}
		template<typename T>
		void SetStoredValue(T value) {
			SetStoredValueInternal(&value);
		}
		template<typename T>
		T GetRuntimeValue() const {
			T value;
			GetRuntimeValue(&value);
			return value;
		}
		template<typename T>
		void SetRuntimeValue(T value) {
			SetRuntimeValueInternal(&value);
		}
		void SetStoredValueRaw(void* src);
	private:
		EntityInstance* entityInstance;
		MonoClassField* monoClassField;
		std::byte* storedValueBuffer = nullptr;

		std::byte* AllocateBuffer(VarFieldType type);
		void SetStoredValueInternal(void* value) const;
		void GetStoredValueInternal(void* value) const;
		void SetRuntimeValueInternal(void* value) const;
		void GetRuntimeValueInternal(void* value) const;

		friend class ScriptEngine;
	};

	using ModuleFieldMap = std::unordered_map<std::string, std::unordered_map<std::string, PublicField>>;

	struct EntityInstanceData {
		EntityInstance instance;
		ModuleFieldMap moduleFieldMap;
	};

	using EntityInstanceMap = std::unordered_map<uint32_t, EntityInstanceData>;
	struct EntityScript {
		std::string moduleName;
		std::string className;
		std::string nameSpace;

		MonoClass* monoClass;
	};

#pragma endregion


	class ScriptEngine : public Module::Module {
	public:
		ScriptEngine();
		~ScriptEngine();

		void Init() override;
		void Close() override;
		void InitAssembly(const char* assemblyPath);
		void LoadRuntimeAssembly(const char* path);

		void ReloadAssembly();
		void ReloadAssembly(const char* assemblyPath);

		bool ModuleExists(const std::string& module);

		void RegisterScripEntity(const std::string& moduleName, uint32_t id);
		void RemoveScriptEntity(const std::string& moduleName, uint32_t id);

		EntityInstanceData& GetInstanceData(uint32_t entity);

	private:
		void MonoInit();
		void MonoShutdown();
		
	};
}