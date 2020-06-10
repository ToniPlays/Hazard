#pragma once
#include "Hazard/Core/Core.h"
#include <iostream>

namespace Hazard {

	class HAZARD_API Module {
	public:
		Module(std::string _name) : name(_name) {}
		virtual ~Module() {};
		bool IsActive() { return enabled; }
		void SetActive(bool active) { enabled = active; }
		std::string GetName() const { return name; }

	public:
		virtual void Awake() {};
		virtual void Start() {};
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void Render() {};
		virtual bool OnEnabled() { return true; };
		virtual bool OnDisabled() { return true; };
		virtual void OnDestroy() {};
	protected:
		std::string name;
		bool enabled = true;
	};
}