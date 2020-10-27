#pragma once
#include "Hazard/Core/Core.h"
#include <yaml-cpp/yaml.h>

namespace Hazard {

	class Entity;
	class HAZARD_API Component {
	public:
		Component(std::string _name) : name(_name) {};
		virtual void OnRender() {};
		std::string GetName() { return name;  }

		virtual void SerializeComponent(YAML::Emitter& out) = 0;
		virtual void DeserializeComponent(YAML::Node in) = 0;
	protected:
		std::string name;
	};
}