
#include "ScriptObject.h"
#include "Core/Metadata/ScriptMetadata.h"


namespace HazardScript 
{
	ScriptObject::~ScriptObject()
	{
		m_Script->RemoveInstance(m_Handle);
#ifdef HZR_INCLUDE_MONO
		mono_gchandle_free(m_Handle);
#endif
	}
	ScriptObject::ScriptObject(ScriptMetadata* script) : m_Script(script)
	{
#ifdef HZR_INCLUDE_MONO
		MonoClass* monoClass = script->GetManagedClass()->Class;
		m_Handle = Mono::InstantiateHandle(monoClass);
#endif
	}
}
