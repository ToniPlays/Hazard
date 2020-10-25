#pragma once
#include "Hazard/Core/Core.h"

namespace Hazard {

	class Entity;
	class HAZARD_API Component {
	public:
		Component(std::string _name) : name(_name) {};
		virtual void OnRender() {};
		std::string GetName() { return name;  }
	protected:
		std::string name;
	};
}