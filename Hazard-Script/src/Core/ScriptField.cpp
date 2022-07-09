
#include "ScriptField.h"
#include "AttributeBuilder.h"

namespace HazardScript 
{
	ScriptField::ScriptField(MonoClassField* field) : m_Field(field)
	{
		m_Name = mono_field_get_name(field);
		m_Visibility = Mono::GetFieldVisibility(field);
		LoadAttributes();
	}
	void ScriptField::LoadAttributes()
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
}