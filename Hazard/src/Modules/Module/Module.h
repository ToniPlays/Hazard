#pragma once
#include "Core/Core.h"

namespace Hazard {

	class HAZARD_API Module {
	public:
		Module(std::string _name) : name(_name) {};
		~Module();

		virtual void OnEnable() {}
		virtual void OnDisable() {}
		virtual void Update() {}
		virtual void LateUpdate() {}
		virtual void OnRender() {};
		virtual void Shutdown() {};

		virtual std::string GetName() { return name; };

	protected:
		std::string name;
	};
}