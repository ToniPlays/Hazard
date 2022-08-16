
#include "ScriptObject.h"
#include "Core/Metadata/ScriptMetadata.h"


namespace HazardScript 
{
	ScriptObject::~ScriptObject()
	{
		m_Script->RemoveInstance(m_Handle);
		std::cout << "Removed script: " << m_Script->GetName() << std::endl;
		mono_gchandle_free(m_Handle);
	}
	ScriptObject::ScriptObject(ScriptMetadata* script) : m_Script(script)
	{
		MonoClass* monoClass = script->GetManagedClass()->Class;
		m_Handle = Mono::InstantiateHandle(monoClass);

		m_Script->RegisterInstance(m_Handle);
	}
}