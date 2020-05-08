#pragma once
#include "hzrpch.h"
#include "JSOn.h"

namespace Hazard {

	json JSON::ReadFile(std::string file)
	{
		std::ifstream f(file);
		json j;
		j << f;
		return j;
	}
}
