
#include "ScriptObject.h"

namespace HazardScript 
{
	ScriptObject::ScriptObject(ScriptMetadata* script) : m_Script(script)
	{
		MonoClass* monoClass = script->GetClass();
		m_Handle = Mono::InstantiateHandle(monoClass);

		m_Script->RegisterInstance(m_Handle);
	}
}