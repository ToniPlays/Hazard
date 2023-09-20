
#include "ScriptObject.h"
#include "Core/Metadata/ScriptMetadata.h"


namespace HazardScript 
{
	ScriptObject::~ScriptObject()
	{
		m_Handle.GetHost()->DestroyInstance(m_Handle);
	}
}
