#pragma once

#include "Mono/Core/Mono.h"
#include "Script.h"

namespace HazardScript 
{
	class ScriptObject 
	{
		friend class Script;
	public:
		ScriptObject() = delete;
		
		~ScriptObject() {
			std::cout << "Removed script: " <<  m_Script->GetName() << std::endl;
			mono_gchandle_free(m_Handle);
		}

		void Invoke(const std::string& name, void** params) {
			m_Script->Invoke(name, GetHandle(), params);
		}
		template<typename T>
		T GetFieldValue(const std::string& name) {
			return m_Script->GetFieldValue<T>(name, GetHandle());
		}
		template<typename T>
		void SetFieldValue(const std::string& name, T value) {
			m_Script->SetFieldValue(name, GetHandle(), &value);
		}
		template<>
		void SetFieldValue(const std::string& name, std::string value) {
			m_Script->SetFieldValue<std::string>(name, GetHandle(), value);
		}

		Script& GetScript() { return *m_Script; }

	private:
		ScriptObject(Script* script);

		MonoObject* GetHandle() { return mono_gchandle_get_target(m_Handle); }

	private:
		uint32_t m_Handle;
		Script* m_Script;
	};
}