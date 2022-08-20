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
		ScriptMetadata(ManagedClass* klass);

		std::string GetName();
		size_t GetFieldCount() { return m_Fields.size(); }
		size_t GetMethodCount() { return m_Methods.size(); }
		std::unordered_map<std::string, FieldMetadata*>& GetFields() { return m_Fields; }

		void UpdateMetadata();
		bool ValidateOrLoadMethod(const std::string& name);

		bool HasField(const std::string& name) {
			return m_Fields.find(name) != m_Fields.end();
		}

		FieldMetadata& GetField(const std::string& name) {
			return *m_Fields[name];
		}

		bool HasMethod(const std::string& name) {
			return m_Methods.find(name) != m_Methods.end();
		}
		
		MethodMetadata& GetMethod(const std::string& name) {
			return *m_Methods[name];
		}
		bool TryInvoke(const std::string& name, MonoObject* target, void** params);
		void Invoke(const std::string& name, MonoObject* target, void** params);

		void RegisterInstance(uint32_t handle)
		{
			m_Instances.push_back(handle);
			for (auto& [name, field] : m_Fields) 
				field->RegisterInstance(handle);
		}

		void RemoveInstance(uint32_t handle)
		{
			auto it = std::find(m_Instances.begin(), m_Instances.end(), handle);
			if (it != m_Instances.end()) 
			{
				m_Instances.erase(it);
				for (auto& [name, field] : m_Fields)
					field->RemoveInstance(handle);
			}
		}

		void SetLive(uint32_t handle, bool live) 
		{
			for (auto& [name, field] : m_Fields)
				field->SetLive(handle, live);
		}


		template<typename T>
		T GetFieldValue(uint32_t handle, const std::string& name, uint32_t index = 0)
		{
			return m_Fields[name]->GetValue<T>(handle, index);
		}
		template<typename T>
		void SetFieldValue(uint32_t handle, const std::string& name, T value, uint32_t index = 0)
		{
			m_Fields[name]->SetValue(handle, value, index);
		}

		void SetArraySize(uint32_t handle, const std::string& name, uint32_t elements) {
			m_Fields[name]->SetArraySize(handle, elements);
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
		ManagedClass* GetManagedClass() { return m_Class; }

	private:
		void LoadFields();
		void LoadMethods();
		void LoadAttributes();

	private:
		ManagedClass* m_Class;

		std::unordered_map<std::string, FieldMetadata*> m_Fields;
		std::unordered_map<std::string, MethodMetadata*> m_Methods;
		std::vector<Attribute*> m_Attributes;
		std::vector<uint32_t> m_Instances;
	};
}