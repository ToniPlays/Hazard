
#include "ScriptMetadata.h"
#include "ScriptAssembly.h"

#include "Core/Instance/ScriptObject.h"
#include "Core/AttributeBuilder.h"
#include "Core/HazardScriptEngine.h"

namespace HazardScript
{
	ScriptMetadata::ScriptMetadata(const Coral::HostInstance& host, Coral::Type* type) : m_Host(host), m_ReflectionType(type)
	{
		HZR_ASSERT(type != nullptr, "");
	}
	std::string ScriptMetadata::GetName()
	{
		return std::string(m_ReflectionType->GetName());
	}
}


