
#include "Script.h"
#include "ScriptObject.h"
#include "AttributeBuilder.h"
#include "ScriptAssembly.h"
#include "HazardScriptEngine.h"

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

	bool Script::ValidateOrLoadMethod(const std::string& name)
	{
		for (ScriptAssembly* assembly : HazardScriptEngine::GetAssemblies()) {
			MonoMethodDesc* desc = mono_method_desc_new(name.c_str(), NULL);
			MonoMethod* method = mono_method_desc_search_in_image(desc, assembly->GetImage());
		

			if (!method) continue;

			//std::string methodName = mono_method_get_reflection_name(method);
			//methodName = methodName.substr(methodName.find_first_of('.') + 1);

			//m_Methods.push_back(methodName);
			return true;
		}
		return false;
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
			ManagedType type = ManagedType::FromType(mono_field_get_type(field));

			std::cout << "Field name " << name << " " << (type.IsValid() ? "Valid" : "Not valid") << std::endl;
		}

	}
	void Script::LoadMethods()
	{
		m_Methods.clear();

		MonoMethod* method = nullptr;
		void* ptr = nullptr;

		while ((method = mono_class_get_methods(m_Class, &ptr))) 
		{
			std::string methodName = mono_method_get_reflection_name(method);
			//methodName = methodName.substr(methodName.find_first_of('.') + 1);
			//m_Methods.push_back(methodName);
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


