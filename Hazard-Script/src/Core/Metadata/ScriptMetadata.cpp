
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
#ifdef HZR_INCLUDE_MONO
		for (Ref<ScriptAssembly> assembly : HazardScriptEngine::GetAssemblies())
		{
			MonoMethodDesc* desc = mono_method_desc_new(name.c_str(), NULL);
			MonoMethod* method = mono_method_desc_search_in_image(desc, assembly->GetImage());

			if (!method) continue;

			m_Methods[name] = Ref<MethodMetadata>::Create(method);
			return true;
		}

#endif
		return false;
	}
#ifdef HZR_INCLUDE_MONO
	bool ScriptMetadata::TryInvoke(const std::string& name, MonoObject* target, void** params)
	{
		if (m_Methods.find(name) == m_Methods.end())
			return false;

		Invoke(name, target, params);
		return true;
	}
	void ScriptMetadata::Invoke(const std::string& name, MonoObject* target, void** params)
	{
		m_Methods[name]->Invoke(target, params);
	}

#endif
	ScriptObject* ScriptMetadata::CreateObject()
	{
		ScriptObject* object = hnew ScriptObject(this);
		RegisterInstance(object->GetHandle(), object);
		return object;
	}
	void ScriptMetadata::LoadFields()
	{
#ifdef HZR_INCLUDE_MONO
		m_Fields.clear();
		MonoClassField* field = nullptr;
		void* ptr = 0;

		while ((field = mono_class_get_fields(m_Class->Class, &ptr)))
		{
			std::string name = mono_field_get_name(field);
			m_Fields[name] = ScriptCache::CacheOrGetFieldMetadata(field);
		}
#endif
	}
	void ScriptMetadata::LoadMethods()
	{
#ifdef HZR_INCLUDE_MONO
		MonoMethod* method = nullptr;
		void* ptr = nullptr;

		while ((method = mono_class_get_methods(m_Class->Class, &ptr)))
		{
			auto m = Ref<MethodMetadata>::Create(method);
			m_Methods[m->GetName()] = m;
		}
#endif
	}
	void ScriptMetadata::LoadAttributes()
	{
		m_Attributes.clear();
#ifdef HZR_INCLUDE_MONO
		MonoCustomAttrInfo* info = mono_custom_attrs_from_class(m_Class->Class);

		if (info == nullptr) return;

		m_Attributes.reserve(info->num_attrs);

		for (int i = 0; i < info->num_attrs; i++)
		{
			MonoCustomAttrEntry entry = info->attrs[i];
			MonoClass* a = mono_method_get_class(entry.ctor);
			MonoObject* obj = mono_custom_attrs_get_attr(info, a);
			MonoClass* attribClass = mono_object_get_class(obj);

			auto attrib = AttributeBuilder::Create(mono_class_get_name(attribClass), obj);
			if (attrib)
				m_Attributes.push_back(attrib);
		}
#endif
	}
}


