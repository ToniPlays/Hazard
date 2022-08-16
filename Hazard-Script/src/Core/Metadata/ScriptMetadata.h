#pragma once

#include "MethodMetadata.h"
#include "Core/Attribute.h"
#include "FieldMetadata.h"
#include "Mono/Core/Mono.h"
#include <unordered_map>

namespace HazardScript 
{
	class ScriptObject;

	class ScriptMetadata
	{
	public:
		ScriptMetadata() = default;
		ScriptMetadata(MonoClass* klass);

		std::string GetName();
		uint32_t GetFieldCount() { return m_Fields.size(); }
		uint32_t GetMethodCount() { return m_Methods.size(); }
		std::unordered_map<std::string, FieldMetadata>& GetFields() { return m_Fields; }

		bool ValidateOrLoadMethod(const std::string& name);

		bool HasField(const std::string& name) {
			return m_Fields.find(name) != m_Fields.end();
		}

		FieldMetadata& GetField(const std::string& name) {
			return m_Fields[name];
		}

		bool HasMethod(const std::string& name) {
			return m_Methods.find(name) != m_Methods.end();
		}
		
		MethodMetadata& GetMethod(const std::string& name) {
			return m_Methods[name];
		}
		bool TryInvoke(const std::string& name, MonoObject* obj, void** params);
		void Invoke(const std::string& name, MonoObject* obj, void** params);

		void RegisterInstance(uint32_t handle)
		{
			m_Instances.push_back(handle);
			for (auto& [name, field] : m_Fields) 
				field.RegisterInstance(handle);
		}

		void RemoveInstance(uint32_t handle)
		{
			auto it = std::find(m_Instances.begin(), m_Instances.end(), handle);
			if (it != m_Instances.end()) 
			{
				m_Instances.erase(it);
				for (auto& [name, field] : m_Fields)
					field.RemoveInstance(handle);
			}
		}

		template<typename T>
		T GetFieldValue(const std::string& name, uint32_t handle) 
		{
			return m_Fields[name].GetValue<T>(handle);
		}
		template<typename T>
		void SetFieldValue(const std::string& name, uint32_t handle, T value)
		{
			m_Fields[name].SetValue(handle, value);
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

		std::unordered_map<std::string, FieldMetadata> m_Fields;
		std::unordered_map<std::string, MethodMetadata> m_Methods;
		std::vector<Attribute*> m_Attributes;
		std::vector<uint32_t> m_Instances;
	};
}