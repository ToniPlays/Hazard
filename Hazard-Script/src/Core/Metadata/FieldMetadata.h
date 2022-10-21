#pragma once

#include "UtilityCore.h"
#include "Core/Attribute.h"
#include "Mono/Core/Mono.h"

#include "ManagedType.h"
#include "Core/FieldValueStorageBase.h"

#include "Buffer.h"
#include <unordered_map>

namespace HazardScript
{
	class FieldMetadata
	{
	public:
		FieldMetadata() = default;
		FieldMetadata(MonoClassField* field);

		std::string GetName() { return m_Name; }
		const uint32_t& GetFlags() { return m_Flags; }
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

		void RegisterInstance(uint32_t handle);
		void RemoveInstance(uint32_t handle);

		void SetLive(uint32_t handle, bool live);

		template<typename T>
		T GetValue(uint32_t handle, uint32_t index = 0)
		{
			MonoObject* obj = mono_gchandle_get_target(handle);
			if (m_Type.IsArray())
				return m_InstanceData[handle].As<ArrayFieldValueStorage>()->GetValueOrDefault<T>(mono_field_get_value_object(Mono::GetDomain(), m_Field, obj), index);
			return m_InstanceData[handle].As<FieldValueStorage>()->GetValue<T>(obj);
		}

		template<typename T>
		void SetValue(uint32_t handle, T value, uint32_t index = 0)
		{
			//Field object
			MonoObject* obj = mono_gchandle_get_target(handle);

			if (m_Type.IsArray()) {
				Ref<ArrayFieldValueStorage> storage = m_InstanceData[handle].As<ArrayFieldValueStorage>();
				if (!storage->IsLive())
					storage->SetStoredValue<T>(index, value);

				//Use array object as target
				storage->SetLiveValue<T>(mono_field_get_value_object(Mono::GetDomain(), m_Field, obj), index, value);
			}
			else
			{
				Ref<FieldValueStorage> storage = m_InstanceData[handle].As<FieldValueStorage>();
				if (!storage->IsLive())
					storage->SetStoredValue<T>(value);
				storage->SetLiveValue<T>(obj, value);
			}
		}

		uint32_t GetElementCount(uint32_t handle);

		void SetArraySize(uint32_t handle, uint32_t elements);
		MonoClassField* GetMonoField() { return m_Field; }

	private:
		void LoadAttributes();

	private:
		MonoClassField* m_Field;
		std::string m_Name;
		ManagedType m_Type;
		uint32_t m_Flags = MonoFlags_Public;
		std::vector<Attribute*> m_Attributes;

		std::unordered_map<uint32_t, Ref<FieldValueStorageBase>> m_InstanceData;
	};
}