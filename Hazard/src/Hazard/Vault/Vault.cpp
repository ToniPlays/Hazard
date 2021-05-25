#pragma once

#include <hzrpch.h>
#include "Vault.h"

namespace Hazard 
{
	std::unordered_map<std::string, RefCount*> Vault::s_Refs;
}