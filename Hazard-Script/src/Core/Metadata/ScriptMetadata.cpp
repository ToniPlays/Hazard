
#include "ScriptMetadata.h"
#include "ScriptAssembly.h"

#include "Core/Instance/ScriptObject.h"
#include "Core/AttributeBuilder.h"
#include "Core/HazardScriptEngine.h"

namespace HazardScript
{
	ScriptMetadata::ScriptMetadata(const Coral::HostInstance& host, const Coral::ReflectionType& type) : m_Host(host), m_ReflectionType(type)
	{

	}
	std::string ScriptMetadata::GetName()
	{
		return m_ReflectionType.Name.ToString();
	}
}


