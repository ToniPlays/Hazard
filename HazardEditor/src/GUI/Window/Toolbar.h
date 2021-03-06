#pragma once
#include "Hazard.h"
#include "GUI/RenderableElement.h"

namespace WindowElement {
	class Toobar : public RenderableElement {
	public:
		Toobar();
		~Toobar();

		void Init() override;
		void OnRender() override;
	};
}