
#include "FieldMetadata.h"
#include "Core/AttributeBuilder.h"


<<<<<<<< HEAD:Hazard-Script/src/Core/Metadata/FieldMetadata.cpp
namespace HazardScript
{
	FieldMetadata::FieldMetadata(MonoClassField* field)
	{
		m_Name = mono_field_get_name(field);
		m_Visibility = Mono::GetFieldVisibility(field);
		m_Type = Mono::GetFieldType(field);
		LoadAttributes();
	}
	void FieldMetadata::LoadAttributes(MonoClassField* field)
========

namespace HazardScript
{
	FieldMetadata::FieldMetadata(MonoClassField* field) : m_Field(field)
	{
		m_Name = mono_field_get_name(field);
		m_Type = ManagedType::FromType(mono_field_get_type(field));

		LoadAttributes();
	}
	void FieldMetadata::LoadAttributes()
>>>>>>>> Scripting:Hazard-Script/src/Core/Metadata/FieldMetada.cpp
	{
		MonoClass* monoClass = mono_field_get_parent(field);
		MonoCustomAttrInfo* info = mono_custom_attrs_from_field(monoClass, field);
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
}
