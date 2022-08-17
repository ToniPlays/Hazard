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

		void RegisterInstance(uint32_t handle);

		void RemoveInstance(uint32_t handle);

		void SetLive(uint32_t handle, bool live);

		template<typename T>
		T GetValue(uint32_t handle, uint32_t index = 0)
		{
			MonoObject* obj = mono_gchandle_get_target(handle);
			if (m_Type.IsArray())
				return m_InstanceData[handle].As<ArrayFieldValueStorage>()->GetValueOrDefault<T>(obj, index);
			return m_InstanceData[handle].As<FieldValueStorage>()->GetValue<T>(obj);
		}

		template<typename T>
		void SetValue(uint32_t handle, T value, uint32_t index = 0)
		{
			MonoObject* obj = mono_gchandle_get_target(handle);

			if (m_Type.IsArray()) {
				Ref<ArrayFieldValueStorage> storage = m_InstanceData[handle].As<ArrayFieldValueStorage>();
				if (!storage->IsLive())
					storage->SetStoredValue<T>(index, value);
				storage->SetLiveValue<T>(obj, index, value);
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
		MonoFlags m_Flags;
		std::vector<Attribute*> m_Attributes;

		std::unordered_map<uint32_t, Ref<FieldValueStorageBase>> m_InstanceData;
	};
}