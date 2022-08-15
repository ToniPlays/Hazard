#pragma once

#include "MethodMetadata.h"
#include "Core/Attribute.h"
#include "FieldMetadata.h"
#include "Core/ManagedType.h"
#include "Mono/Core/Mono.h"
#include <unordered_map>

namespace HazardScript 
{
	class ScriptObject;

	//This should be kind of a metadata class with no real functionality
	class ScriptMetadata 
	{
	public:
		ScriptMetadata() = default;
		ScriptMetadata(MonoClass* klass);

		std::string GetName();
		uint32_t GetFieldCount() { return m_Fields.size(); }
		uint32_t GetMethodCount() { return m_Methods.size(); }
		std::unordered_map<std::string, FieldMetadata> GetFields() { return m_Fields; }

		bool HasField(const std::string& name) 
		{
			return m_Fields.find(name) != m_Fields.end();
		}

		bool HasMethod(const std::string& name) 
		{
			return m_Methods.find(name) != m_Methods.end();
		}

		//Attribute stuff
		template<typename T>
		bool Has() const {
			for (Attribute* attrib : m_Attributes) {
				if (attrib->GetAttributeType() == T::GetStaticType()) 
					return true;
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

		Ref<ScriptObject> CreateObject();
		MonoClass* GetClass() { return m_ManagedClass->Class; }

	private:
		void LoadFields();
		void LoadMethods();
		void LoadAttributes();

	private:
		ManagedClass* m_ManagedClass = nullptr;
		std::unordered_map<std::string, FieldMetadata> m_Fields;
		std::unordered_map<std::string, ManagedType> m_Methods;
		std::vector<Attribute*> m_Attributes;
	};
} 