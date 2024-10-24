#pragma once

#include "UtilityCore.h"
#include "Core/Attribute.h"
#include "Mono/Core/Mono.h"

#include "ManagedType.h"
#include "Core/FieldValueStorageBase.h"
#include "Core/ValueWrapper.h"

#include <unordered_map>

namespace HazardScript
{
	class FieldMetadata : public RefCount
	{
	public:
		FieldMetadata() = default;
		FieldMetadata(MonoClassField* field);

		std::string GetName() const { return m_Name; }
		const uint32_t& GetFlags() const { return m_Flags; }
		const ManagedType& GetType() const { return m_Type; }

		template<typename T>
		bool Has() const 
		{
			for (Ref<Attribute> attrib : m_Attributes) 
			{
				if (attrib->GetAttributeType() == T::GetStaticType()) 
					return true;
			}
			return false;
		}
		template<typename T>
		const Ref<T> Get() 
		{
			for (Ref<Attribute> attrib : m_Attributes) 
			{
				if (attrib->GetAttributeType() == T::GetStaticType()) 
					return attrib.As<T>();
			}

			return Ref<T>();
		}

		void RegisterInstance(uint32_t handle);
		void RemoveInstance(uint32_t handle);

		void SetLive(uint32_t handle, bool live);

		template<typename T>
		T GetValue(uint32_t handle, uint32_t index = 0)
		{
			MonoObject* obj = mono_gchandle_get_target(handle);
			Buffer value = m_InstanceData[handle]->GetValueOrDefault(obj);
			return ValueWrapper(value.Data, value.Size).Get<T>();
		}

		template<typename T>
		void SetValue(uint32_t handle, T value, uint32_t index = 0)
		{
			HZR_ASSERT(m_InstanceData.find(handle) != m_InstanceData.end(), "Handle not found");
			Buffer val = { &value, sizeof(T) };
			MonoObject* obj = mono_gchandle_get_target(handle);
			m_InstanceData[handle]->SetData(obj, index, val);

			/*//Field object
			if (m_Type.IsArray()) 
			{
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
			}*/
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
		std::vector<Ref<Attribute>> m_Attributes;
		std::unordered_map<uint32_t, Ref<FieldValueStorageBase>> m_InstanceData;
	};
}