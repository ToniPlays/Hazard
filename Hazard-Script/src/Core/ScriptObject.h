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
		
		void Invoke(const std::string& name, void* params) {
			m_Script->Invoke(name, GetObject(), params);
		}

		Script& GetScript() { return *m_Script; }


	private:

		ScriptObject(Script* script);

		MonoObject* GetObject() { return mono_gchandle_get_target(m_Handle); }

	private:
		uint32_t m_Handle;
		Script* m_Script;
	};
}