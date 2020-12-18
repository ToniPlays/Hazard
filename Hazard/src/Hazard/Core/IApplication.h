#pragma once

#include "Core.h"
#include "Hazard/Core/Events/Event.h"

namespace Hazard {
	class HAZARD_API IApplication {
	public:
		virtual void Awake() {};
		virtual void Start() {};
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual bool OnEvent(Event& e) { return true; };
		virtual void OnClose() {};
	};
}