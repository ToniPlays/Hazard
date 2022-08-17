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
		~ScriptObject();
		
		void TryInvoke(const std::string& name, void** params = nullptr) 
		{
			m_Script->TryInvoke(name, GetHandle(), params);
		}
		void Invoke(const std::string& name, void** params = nullptr) 
		{
			m_Script->Invoke(name, GetHandle(), params);
		}
		
		template<typename T>
		T GetFieldValue(const std::string& name, uint32_t index = 0)
		{
			return m_Script->GetFieldValue<T>(m_Handle, name, index);
		}
		template<typename T>
		void SetFieldValue(const std::string& name, T value, uint32_t index = 0) 
		{
			m_Script->SetFieldValue(m_Handle, name, value, index);
		}
		
		void SetArraySize(const std::string& name, uint32_t elements) {
			m_Script->SetArraySize(m_Handle, name, elements);
		}

		uint32_t GetFieldValueCount(const std::string& name) { return m_Script->GetField(name).GetElementCount(m_Handle); }
		ScriptMetadata& GetScript() { return *m_Script; }
		bool IsValid() { return m_Script; }

	private:
		ScriptObject(ScriptMetadata* script);
		MonoObject* GetHandle() { return mono_gchandle_get_target(m_Handle); }

	private:
		uint32_t m_Handle = 0;
		ScriptMetadata* m_Script;
	};
}