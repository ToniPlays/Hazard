#pragma once

#include <hzrpch.h>
#include "GUI/RenderableElement.h"

namespace WindowElement {
	class Toolbar : public RenderableElement {
	public:
		Toolbar();
		~Toolbar();

		void Init() override;
		void OnRender() override;
		bool OnEvent(Event& e) override;
		//bool OnKey(Hazard::KeyPressedEvent& e);
	private:
		void SetPlaying(bool playing);
	};
}