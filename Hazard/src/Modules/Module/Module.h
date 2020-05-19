#pragma once
#include "Core/Core.h"
#include "Events/Event.h"

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
		virtual bool OnEvent(Event& e) { return false; };
		virtual std::string GetName() { return name; };

		void SetActive(bool active) { isActive = active; }
		bool IsActive() { return isActive; }

	protected:
		std::string name;
		bool isActive = true;
	};
}