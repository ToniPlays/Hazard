#pragma once
#include "Core/Core.h"
#include "Core/Layers/Layer.h"

namespace Hazard {

	class HAZARD_API ImGuiLayer : public Layer {

	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnImGuiRender();
		void OnEvent(Event& e) override;

		void Begin();
		void End();
	private:
		float m_time = 0.0f;
	};
}