#pragma once

#include "Method.h"
#include "Attribute.h"
#include "ScriptField.h"
#include "Mono/Core/Mono.h"
#include <unordered_map>

namespace HazardScript 
{
	class ScriptObject;


	class Script 
	{
	public:
		Script() = default;
		Script(MonoClass* klass);

		std::string GetName();
		uint32_t GetFieldCount() { return m_FieldCount; }
		uint32_t GetMethodCount() { return m_MethodCount; }

		bool HasField(const std::string& name) {
			return m_Fields.find(name) != m_Fields.end();
		}

		ScriptField GetField(const std::string& name) {
			return m_Fields[name];
		}

		bool HasMethod(const std::string& name) {
			return m_Methods.find(name) != m_Methods.end();
		}
		
		Method GetMethod(const std::string& name) {
			return m_Methods[name];
		}

		void Invoke(const std::string& name, MonoObject* obj, void** params);
		template<typename T>
		T GetFieldValue(const std::string& name, MonoObject* obj) {
			T value;
			mono_field_get_value(obj, m_Fields[name].GetField(), &value);
			return value;
		}
		void SetFieldValue(const std::string& name, MonoObject* obj, void* value) {
			mono_field_set_value(obj, m_Fields[name].GetField(), value);
		}

		template<typename T>
		bool Has() const {
			for (Attribute* attrib : m_Attributes) {
				if (attrib->GetAttributeType() == T::GetStaticType()) return true;
			}
			return false;
		}
		template<typename T>
		const T& Get() {
			for (Attribute* attrib : m_Attributes) {
				if (attrib->GetAttributeType() == T::GetStaticType()) return dynamic_cast<T&>(*attrib);
			}
			return T();
		}

		ScriptObject* CreateObject();
		MonoClass* GetClass() { return m_Class; }


	private:
		void LoadFields();
		void LoadMethods();
		void LoadAttributes();

	private:
		MonoClass* m_Class;
		
		uint32_t m_FieldCount, m_MethodCount;

		std::unordered_map<std::string, ScriptField> m_Fields;
		std::unordered_map<std::string, Method> m_Methods;
		std::vector<Attribute*> m_Attributes;
	};
}