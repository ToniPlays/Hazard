#pragma once
#include <hzrpch.h>
#include "Module.h"

namespace Hazard {
	Module::Module(std::string _name) : name(_name) {
#ifdef HZR_DEBUG
		HZR_CORE_INFO("Created module" + name);
#endif // HZR_DEBUG

	}
	Module::~Module()
	{

	}
}