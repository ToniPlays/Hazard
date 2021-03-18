#pragma once

#include "Hazard/Core/Core.h"
#include "ScriptUtils.h"
#include "Hazard/Entity/Scene.h"

extern "C"
{
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoMethod MonoMethod;
}

namespace Hazard::Scripting {

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
		MonoMethod* OnCreate = nullptr;
		MonoMethod* OnStart = nullptr;
		MonoMethod* OnUpdate = nullptr;
		MonoMethod* OnLateUpdate = nullptr;
		MonoMethod* OnFixedUpdate = nullptr;

		void InitClassMethods();
	};

}