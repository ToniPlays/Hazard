#pragma once
#include "Hazard/Core/Core.h"
#include <yaml-cpp/yaml.h>

namespace Hazard {

	class Entity;
	class HAZARD_API Component {
	public:
		Component(std::string _name, Entity* parent = nullptr) : name(_name) {
			this->parent = parent;
		};
		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnRender() {};
		std::string GetName() { return name;  }

		virtual void SerializeComponent(YAML::Emitter& out) = 0;
		virtual void DeserializeComponent(YAML::Node in) = 0;
		virtual void PostDeserialize() {};
		void SetParent(Entity* entity) { parent = entity; }
	protected:

		Entity* parent;
		std::string name;
	};
}