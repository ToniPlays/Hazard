#pragma once

#include <hzrpch.h>
#include "Vault.h"

namespace Hazard 
{
	std::unordered_map<std::string, RefCount*> Vault::m_Refs;
}