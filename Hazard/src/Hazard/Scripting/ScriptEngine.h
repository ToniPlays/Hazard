#pragma once

#include "Hazard/Module/Module.h"
#include "Hazard/Entity/Scene.h"
#include "ScriptUtils.h"


extern "C"
{
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoMethod MonoMethod;
}


namespace Hazard::Scripting {


#pragma region PUBLIC_FIELD

	struct EntityScript;

	struct EntityInstance
	{
		EntityScript* ScriptClass;
		uint32_t handle = 0;
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
			GetRuntimeValueInternal(&value);
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
		MonoMethod* Constructor = nullptr;
		MonoMethod* OnUpdate = nullptr;

		void InitClassMethods();
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

		void ReloadRuntimeAssembly();
		void ReloadRuntimeAssembly(const char* assemblyPath);

		bool ModuleExists(const std::string& module);

		void RegisterScripEntity(const std::string& moduleName, uint32_t id);
		void RemoveScriptEntity(const std::string& moduleName, uint32_t id);

		void InstantiateScriptEntity(uint32_t handle, const std::string& moduleName);

		EntityInstanceData& GetInstanceData(uint32_t entity);

		bool GetRunning() { return running; }
		void SetRunning(bool running) { this->running = running; }

		void Step();

		void EntityStart(EntityInstance& instance);

	private:
		void MonoInit();
		void MonoShutdown();

		uint32_t Instantiate(EntityScript& instance);
		
		bool running = false;
	};
}