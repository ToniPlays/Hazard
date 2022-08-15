
#include "ScriptMetadata.h"
#include "Core/Instance/ScriptObject.h"
#include "Core/AttributeBuilder.h"
#include "ScriptAssembly.h"
#include "Core/HazardScriptEngine.h"
#include "Core/Attribute.h"
#include "Core/ScriptCache.h"

#include <sstream>

namespace HazardScript
{
	ScriptMetadata::ScriptMetadata(MonoClass* klass)
	{
		m_ManagedClass = ScriptCache::GetManagedClass(klass);

		LoadFields();
		LoadMethods();
		LoadAttributes();
	}
	std::string ScriptMetadata::GetName()
	{
		return mono_class_get_name(m_ManagedClass->Class);
	}

	Ref<ScriptObject> ScriptMetadata::CreateObject()
	{
		return Ref<ScriptObject>::Create(this);
	}
	void ScriptMetadata::LoadFields()
	{
		MonoClassField* field = nullptr;
		void* ptr = 0;

		while ((field = mono_class_get_fields(m_ManagedClass->Class, &ptr))) {

			std::string name = mono_field_get_name(field);
			MonoType* monoType = mono_field_get_type(field);
			ManagedType type = ManagedType::FromType(monoType);

			m_Fields[name] = FieldMetadata(field);
		}
	}
	void ScriptMetadata::LoadMethods()
	{
		m_Methods.clear();

		MonoMethod* method = nullptr;
		void* ptr = nullptr;

		while ((method = mono_class_get_methods(m_ManagedClass->Class, &ptr)))
		{
			std::string methodName = mono_method_get_reflection_name(method);
			//methodName = methodName.substr(methodName.find_first_of('.') + 1);
			//m_Methods.push_back(methodName);
		}
	}
	void ScriptMetadata::LoadAttributes()
	{
		m_Attributes.clear();
		MonoCustomAttrInfo* info = mono_custom_attrs_from_class(m_ManagedClass->Class);

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


