#pragma once

#include "UtilityCore.h"
#include "Core/Attribute.h"

#include "Core/FieldValueStorageBase.h"
#include "Core/ValueWrapper.h"

#include "Coral/ManagedField.hpp"
#include "Coral/ManagedObject.hpp"
#include "Coral/StringHelper.hpp"

#include <unordered_map>

namespace HazardScript
{
	class FieldMetadata : public RefCount
	{
	public:
		FieldMetadata() = default;
		//FieldMetadata(MonoClassField* field);

		std::string GetName() const { return Coral::StringHelper::ConvertWideToUtf8(m_Field.Name); }
		Coral::TypeVisibility GetTypeVisibility() const { return m_Field.visibility; }

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

		template<typename T>
		T GetValue(const Coral::ManagedObject& handle, uint32_t index = 0)
		{
			//MonoObject* obj = mono_gchandle_get_target(handle);
			Buffer value;// = m_InstanceData[handle]->GetValueOrDefault(obj);
			return ValueWrapper(value.Data, value.Size).Get<T>();
		}

		template<typename T>
		void SetValue(const Coral::ManagedObject& handle, T value, uint32_t index = 0)
		{
			/*
			HZR_ASSERT(m_InstanceData.find(handle) != m_InstanceData.end(), "Handle not found");
			Buffer val = { &value, sizeof(T) };
			MonoObject* obj = mono_gchandle_get_target(handle);
			m_InstanceData[handle]->SetData(obj, index, val);
			*/
		}

		uint32_t GetElementCount(const Coral::ManagedObject& handle);

		void SetArraySize(const Coral::ManagedObject& handle, uint32_t elements);

	private:
		void LoadAttributes();

	private:
		Coral::ManagedField m_Field;

		std::vector<Ref<Attribute>> m_Attributes;
		std::unordered_map<uint32_t, Ref<FieldValueStorageBase>> m_InstanceData;
	};
}
