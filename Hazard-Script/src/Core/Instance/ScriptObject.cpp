#include "ScriptObject.h"

namespace HazardScript 
{
	ScriptObject::ScriptObject(ScriptMetadata* script) : m_Script(script) 
	{
		m_Handle = Mono::InstantiateHandle(script->GetClass());
		//LoadFields(script->GetFields());
	}
	void ScriptObject::LoadFields(std::unordered_map<std::string, ScriptField>& fields)
	{

	}
}