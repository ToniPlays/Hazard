
#include "FieldMetadata.h"
#include "Core/AttributeBuilder.h"
#include "Core/FieldValueStorage.h"

namespace HazardScript
{
	FieldMetadata::FieldMetadata(MonoClassField* field) : m_Field(field)
	{
		m_Name = mono_field_get_name(field);
		m_Type = ManagedType::FromType(mono_field_get_type(field));

		LoadAttributes();
	}
	uint32_t FieldMetadata::GetElementCount(uint32_t handle)
	{
		if (!m_Type.IsArray()) return 1;
		return m_InstanceData[handle].As<ArrayFieldValueStorage>()->GetLength();
	}
	void FieldMetadata::SetArraySize(uint32_t handle, uint32_t elements)
	{
		HZR_ASSERT(m_Type.IsArray(), "Attempted to set array size of non array type");
		m_InstanceData[handle].As<ArrayFieldValueStorage>()->Resize(elements);
	}
	void FieldMetadata::LoadAttributes()
	{
		MonoClass* monoClass = mono_field_get_parent(m_Field);
		MonoCustomAttrInfo* info = mono_custom_attrs_from_field(monoClass, m_Field);
		if (info == nullptr) return;

		m_Attributes.reserve(info->num_attrs);

		for (uint32_t i = 0; i < info->num_attrs; i++)
		{
			MonoCustomAttrEntry entry = info->attrs[i];
			MonoClass* a = mono_method_get_class(entry.ctor);
			MonoObject* obj = mono_custom_attrs_get_attr(info, a);
			MonoClass* attribClass = mono_object_get_class(obj);

			m_Attributes.push_back(AttributeBuilder::Create(mono_class_get_name(attribClass), obj));
		}
	}
	void FieldMetadata::RegisterInstance(uint32_t handle)
	{
		if (m_Type.IsArray())
			m_InstanceData[handle] = Ref<ArrayFieldValueStorage>::Create(this);
		else
			m_InstanceData[handle] = Ref<FieldValueStorage>::Create(this);
	}
	void FieldMetadata::RemoveInstance(uint32_t handle)
	{
		m_InstanceData.erase(handle);
	}
	void FieldMetadata::SetLive(uint32_t handle, bool live) 
	{
		m_InstanceData[handle]->SetLive(live);
	}
}