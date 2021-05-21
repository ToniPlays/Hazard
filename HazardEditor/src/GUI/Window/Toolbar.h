#pragma once
#include "Hazard.h"
#include "GUI/RenderableElement.h"

namespace WindowElement {
	class Toolbar : public RenderableElement {
	public:
		Toolbar();
		~Toolbar();

		void Init() override;
		void OnRender() override;
		bool OnEvent(Event& e) override;
		bool OnKey(KeyPressedEvent& e);
	private:
		bool dontFocusAfterStop = false;
	};
}