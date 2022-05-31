#pragma once

#include "Method.h"
#include "Attribute.h"
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

		void Invoke(const std::string& name, MonoObject* obj, void* params);

		ScriptObject* CreateObject();
		MonoClass* GetClass() { return m_Class; }

	private:
		void LoadFields() {};
		void LoadMethods();
		void LoadAttributes();

	private:
		MonoClass* m_Class;
		
		uint32_t m_FieldCount, m_MethodCount;

		//std::vector<ScriptField> m_Fields;
		std::unordered_map<std::string, Method> m_Methods;
		std::vector<Attribute*> m_Attributes;
	};
}