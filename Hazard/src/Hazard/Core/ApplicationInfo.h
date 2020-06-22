#pragma once

#include "Core.h"
#include <hzrpch.h>

namespace Hazard {

	class HAZARD_API ApplicationInfo {
		friend class HazardLoop;
	public:
		bool ShouldClose() { return shouldClose; }
		void SetValue(std::string key, std::string value) {
			data[key] = value;
		};
		std::string Get(std::string key) {
			return data[key];
		}

	public:
		std::unordered_map<std::string, std::string> data;
	private:
		bool shouldClose = false;
	};
}