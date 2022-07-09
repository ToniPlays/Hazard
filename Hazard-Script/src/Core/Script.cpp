
#include "Script.h"
#include "ScriptObject.h"
#include "AttributeBuilder.h"

namespace HazardScript 
{
	Script::Script(MonoClass* klass) : m_Class(klass)
	{
		LoadFields();
		LoadMethods();
		LoadAttributes();
	}
	std::string Script::GetName()
	{
		return mono_class_get_name(m_Class);
	}
	void Script::Invoke(const std::string& name, MonoObject* obj, void** params)
	{
		m_Methods[name].Invoke(obj, params);
	}
	ScriptObject* Script::CreateObject()
	{
		return new ScriptObject(this);
	}
	void Script::LoadFields() 
	{
		MonoClassField* field = nullptr;
		void* ptr = 0;

		while ((field = mono_class_get_fields(m_Class, &ptr))) {
			std::string name = mono_field_get_name(field);
			m_Fields[name] = ScriptField(field);
		}

	}
	void Script::LoadMethods()
	{
		m_Methods.clear();

		MonoMethod* method = nullptr;
		void* ptr = nullptr;

		while ((method = mono_class_get_methods(m_Class, &ptr))) {
			Method m(method);
			m_Methods[m.GetName()] = m;
		}
	}
	void Script::LoadAttributes()
	{
		m_Attributes.clear();
		MonoCustomAttrInfo* info = mono_custom_attrs_from_class(m_Class);

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

