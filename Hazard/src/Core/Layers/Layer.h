#pragma once

#include "Core/Core.h"
#include "Core/EventSystem/Event.h"
#include "Core/Window/Window.h"

namespace Hazard {
	class HAZARD_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& e) {};

		inline const std::string& GetName() const { return DebugName; }
	private:
		std::string DebugName;
	};
}