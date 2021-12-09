#pragma once

#include "Hazard/Core/Core.h"
#include "Attributes/Attribute.h"
#include "Script.h"

namespace Hazard::Scripting 
{
	struct ScriptFieldMetadata
	{
		std::string Name;
		std::string CustomType;
		FieldType Type;
		FieldVisibility Visibility;
		std::vector<Attribute*> FieldAttributes;

		template<typename T>
		bool Has() {
			for (Attribute* attrib : FieldAttributes) {
				if (attrib->GetAttributeType() == T::GetStaticType())
					return true;
			}
			return false;
		}
		template<typename T>
		T& Get() {
			for (Attribute* attrib : FieldAttributes)
			{
				if (attrib->GetAttributeType() == T::GetStaticType())
					return dynamic_cast<T&>(*attrib);
			}
			return T();
		}
	};


	struct ScriptMethodMetadata
	{
		std::string Name;
		FieldVisibility Visibility;
		std::vector<Attribute*> MethodAttributes;

		template<typename T>
		bool Has() {
			for (Attribute* attrib : MethodAttributes) {
				if (attrib->GetAttributeType() == T::GetStaticType())
					return true;
			}
			return false;
		}
		template<typename T>
		T& Get() {
			for (Attribute* attrib : MethodAttributes)
			{
				if (attrib->GetAttributeType() == T::GetStaticType())
					return dynamic_cast<T&>(*attrib);
			}
			return T();
		}
	};

	struct ScriptMetadata 
	{
		std::string ModuleName;
		std::string ClassName;
		std::string Namespace;
		uint32_t Count = 0;

		std::vector<Attribute*> ClassAttributes;
		std::vector<ScriptFieldMetadata> Fields;
		std::vector<ScriptMethodMetadata> Methods;

		template<typename T>
		bool Has() {
			for (Attribute* attrib : ClassAttributes) {
				if (attrib->GetAttributeType() == T::GetStaticType())
					return true;
			}
			return false;
		}
		template<typename T>
		T& Get() {
			for (Attribute* attrib : ClassAttributes) 
			{
				if (attrib->GetAttributeType() == T::GetStaticType())
					return dynamic_cast<T&>(*attrib);
			}
			return T();
		}
	};

	struct InstanceData 
	{
		uint32_t Handle = 0;
		std::vector<Script*> Scripts;
	};

	class ScriptRegistry 
	{
	public:
		ScriptRegistry() = default;
		
		bool HasScript(const std::string& script);
		bool HasInstance(uint32_t handle);
		ScriptMetadata& GetScriptMetadata(const std::string& script);

		void Add(ScriptMetadata script) { m_Scripts[script.ModuleName] = script; };
		void Remove(uint32_t handle);
		void RegisterEntityScript(uint32_t handle, const std::string& moduleName);
		void RegisterEntityScript(uint32_t handle, Script* script);

		InstanceData& GetInstanceData(uint32_t handle) {
			HZR_CORE_ASSERT(m_InstanceData.find(handle) != m_InstanceData.end(), "Handle does not exist");
			return m_InstanceData[handle]; 
		}

		void Clear() { m_Scripts.clear(); }
		std::unordered_map<std::string, ScriptMetadata> GetScripts() { return m_Scripts; }
		std::unordered_map<uint32_t, InstanceData> GetRegisteredInstances() { return m_InstanceData; }

	private:
		std::unordered_map<std::string, ScriptMetadata> m_Scripts;
		std::unordered_map<uint32_t, InstanceData> m_InstanceData;
	};
}