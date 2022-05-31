#include "ScriptObject.h"

namespace HazardScript 
{
	ScriptObject::ScriptObject(Script* script) : m_Script(script) 
	{
		MonoClass* monoClass = script->GetClass();
		m_Handle = Mono::InstantiateHandle(monoClass);
	}
}