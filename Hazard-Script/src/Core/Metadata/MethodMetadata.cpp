#include "MethodMetadata.h"
#include "Core/AttributeBuilder.h"
#include "Core/HazardScriptEngine.h"

namespace HazardScript
{
	MethodMetadata::MethodMetadata(MonoMethod* method) : m_Method(method)
	{
		Init();
		LoadAttributes();
	}
	void MethodMetadata::Init()
	{
		m_Name = mono_method_get_reflection_name(m_Method);
		m_Name = m_Name.substr(m_Name.find_first_of('.') + 1);
		m_Visibility = Mono::GetMethodVisibility(m_Method);
	}
	void MethodMetadata::LoadAttributes()
	{
		m_Attributes.clear();
		MonoCustomAttrInfo* info = mono_custom_attrs_from_method(m_Method);

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
