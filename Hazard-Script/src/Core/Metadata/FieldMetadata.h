#pragma once

#include "UtilityCore.h"
#include "Core/Attribute.h"
#include "Mono/Core/Mono.h"

#include "Core/ValueWrapper.h"

#include "ManagedType.h"
#include "Core/FieldValueStorage.h"

#include "Buffer.h"
#include <unordered_map>

namespace HazardScript
{
	class FieldMetadata
	{
	public:
		FieldMetadata() = default;
		FieldMetadata(MonoClassField* field);
		~FieldMetadata() {
			
		}

		std::string GetName() { return m_Name; }
		const MonoFlags& GetFlags() { return m_Flags; }
		const ManagedType& GetType() { return m_Type; }

		template<typename T>
		bool Has() const {
			for (Attribute* attrib : m_Attributes) {
				if (attrib->GetAttributeType() == T::GetStaticType()) return true;
			}
			return false;
		}
		template<typename T>
		const T& Get() {
			for (Attribute* attrib : m_Attributes) {
				if (attrib->GetAttributeType() == T::GetStaticType()) return dynamic_cast<T&>(*attrib);
			}
			return T();
		}

		void RegisterInstance(uint32_t handle)
		{
			if (m_Type.IsArray())
				m_InstanceData[handle] = Ref<ArrayFieldValueStorage>::Create();
			else
				m_InstanceData[handle] = Ref<FieldValueStorage>::Create(&m_Type);
		}
		void RemoveInstance(uint32_t handle)
		{
			m_InstanceData.erase(handle);
		}

		template<typename T>
		T GetValue(uint32_t handle, uint32_t index = 0)
		{
			if (m_Type.IsArray())
			{
				return m_InstanceData[handle].As<ArrayFieldValueStorage>()->GetValue<T>(index);
			}

			if (m_Type.IsReference())
			{
				struct ObjectReferenceData {
					MonoObject* obj;
					uint32_t handle;
					T ID;
				};
				ObjectReferenceData data = m_InstanceData[handle].As<FieldValueStorage>()->GetValueOrDefault<ObjectReferenceData>();
				return data.ID;
			}
			return m_InstanceData[handle].As<FieldValueStorage>()->GetValue<T>();
		}
		template<typename T>
		void SetValue(uint32_t handle, T value, uint32_t index = 0)
		{
			MonoObject* target = mono_gchandle_get_target(handle);
			if (m_Type.IsArray())
			{
				m_InstanceData[handle].As<ArrayFieldValueStorage>()->SetValue<T>(index, value);
				return;
			}

			Ref<FieldValueStorage> storage = m_InstanceData[handle].As<FieldValueStorage>();

			if (m_Type.IsReference())
			{
				struct ObjectReferenceData {
					MonoObject* obj;
					uint32_t handle;
					T ID;
				};

				if (storage->HasValue())
				{
					ObjectReferenceData data = storage->GetValue<ObjectReferenceData>();
					mono_gchandle_free(data.handle);
				}

				ObjectReferenceData data;
				data.ID = value;

				data.handle = Mono::InstantiateHandle(m_Type.TypeClass->Class);
				data.obj = mono_gchandle_get_target(data.handle);

				mono_field_set_value(target, m_Field, data.obj);
				storage->SetValue<ObjectReferenceData>(data);

				void* params[] = { &value };
				MonoMethod* ctor = mono_class_get_method_from_name(m_Type.TypeClass->Class, ".ctor", 1);
				mono_runtime_invoke(ctor, data.obj, params, nullptr);
			}
			else
			{
				storage->SetValue<T>(value);
				mono_field_set_value(target, m_Field, &value);
			}
		}

	private:
		void LoadAttributes();

	private:
		MonoClassField* m_Field;
		std::string m_Name;
		ManagedType m_Type;
		MonoFlags m_Flags;
		std::vector<Attribute*> m_Attributes;

		std::unordered_map<uint32_t, Ref<FieldValueStorageBase>> m_InstanceData;
	};
}