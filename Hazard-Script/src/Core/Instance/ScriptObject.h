#pragma once

#include "Mono/Core/Mono.h"
#include "Core/Metadata/ScriptMetadata.h"
#include "Ref.h"

namespace HazardScript 
{
	class ScriptObject : public RefCount
	{
		friend class ScriptMetadata;

	public:
		ScriptObject() = delete;
		ScriptObject(ScriptMetadata* script);
		
		~ScriptObject() 
		{
			mono_gchandle_free(m_Handle);
		}

		std::unordered_map<std::string, ManagedField> GetFields() { return m_Fields; }

		ScriptMetadata& GetScript() { return *m_Script; }

	private:
		MonoObject* GetHandle() { return mono_gchandle_get_target(m_Handle); }

		void LoadFields(std::unordered_map<std::string, ScriptField>& fields);

	private:
		uint32_t m_Handle;
		ScriptMetadata* m_Script;

		std::unordered_map<std::string, ManagedField> m_Fields;
	};
}