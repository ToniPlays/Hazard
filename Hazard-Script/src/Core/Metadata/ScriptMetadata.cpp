
#include "ScriptMetadata.h"
#include "ScriptAssembly.h"

#include "Core/Instance/ScriptObject.h"
#include "Core/AttributeBuilder.h"
#include "Core/HazardScriptEngine.h"

#include "Core/ScriptCache.h"

namespace HazardScript
{
	ScriptMetadata::ScriptMetadata(ManagedClass* klass) : m_Class(klass)
	{

	}
	std::string ScriptMetadata::GetName()
	{
		return m_Class->FullName;
	}

	void ScriptMetadata::UpdateMetadata()
	{
		LoadFields();
		LoadMethods();
		LoadAttributes();
	}

	bool ScriptMetadata::ValidateOrLoadMethod(const std::string& name)
	{
		for (ScriptAssembly* assembly : HazardScriptEngine::GetAssemblies()) {
			MonoMethodDesc* desc = mono_method_desc_new(name.c_str(), NULL);
			MonoMethod* method = mono_method_desc_search_in_image(desc, assembly->GetImage());

			if (!method) continue;

			MethodMetadata* m = new MethodMetadata(method);
			m_Methods[name] = m;
			return true;
		}
		return false;
	}
	bool ScriptMetadata::TryInvoke(const std::string& name, MonoObject* obj, void** params)
	{
		if (m_Methods.find(name) == m_Methods.end()) 
			return false;

		Invoke(name, obj, params);
		return true;
	}
	void ScriptMetadata::Invoke(const std::string& name, MonoObject* obj, void** params)
	{
		m_Methods[name]->Invoke(obj, params);
	}

	ScriptObject* ScriptMetadata::CreateObject()
	{
		return new ScriptObject(this);
	}
	void ScriptMetadata::LoadFields()
	{
		m_Fields.clear();
		MonoClassField* field = nullptr;
		void* ptr = 0;

		while ((field = mono_class_get_fields(m_Class->Class, &ptr))) 
		{
			std::string name = mono_field_get_name(field);
			m_Fields[name] = ScriptCache::CacheOrGetFieldMetadata(field);
			if (m_Fields[name]->GetType().NativeType == NativeType::None) {
				std::cout << name << std::endl;
			}
		}
	}
	void ScriptMetadata::LoadMethods()
	{
		MonoMethod* method = nullptr;
		void* ptr = nullptr;

		while ((method = mono_class_get_methods(m_Class->Class, &ptr))) {
			MethodMetadata* m = new MethodMetadata(method);
			m_Methods[m->GetName()] = m;
		}
	}
	void ScriptMetadata::LoadAttributes()
	{
		m_Attributes.clear();
		MonoCustomAttrInfo* info = mono_custom_attrs_from_class(m_Class->Class);

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


