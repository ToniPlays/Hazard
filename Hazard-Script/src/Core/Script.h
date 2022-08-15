#pragma once

#include "Method.h"
#include "Attribute.h"
#include "ScriptField.h"
#include "ManagedType.h"
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
		uint32_t GetFieldCount() { return m_Fields.size(); }
		uint32_t GetMethodCount() { return m_Methods.size(); }
		std::unordered_map<std::string, ManagedType> GetFields() { return m_Fields; }

		bool ValidateOrLoadMethod(const std::string& name);

		bool HasField(const std::string& name) 
		{
			return m_Fields.find(name) != m_Fields.end();
		}

		bool HasMethod(const std::string& name) 
		{
			return m_Methods.find(name) != m_Fields.end();
		}


		//Attribute stuff
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

		std::unordered_map<std::string, ManagedType> m_Fields;
		std::unordered_map<std::string, ManagedType> m_Methods;
		std::vector<Attribute*> m_Attributes;
	};
} 