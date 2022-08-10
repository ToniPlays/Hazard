#pragma once

#include "Hazard/Assets/Asset.h"
#include "HazardScript.h"

namespace Hazard 
{
	class HScript : public Asset 
	{
	public:
		HScript(const std::string& moduleName) : m_ModuleName(moduleName) {};
		std::string GetModuleName() { return m_ModuleName; }

	private:
		std::string m_ModuleName;
	};
}