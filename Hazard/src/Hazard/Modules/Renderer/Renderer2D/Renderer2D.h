#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard {
	class HAZARD_API Renderer2D {
	public:
		Renderer2D();
		~Renderer2D();

		void Render();
		void OnResized(Event& e);

	private:

	};
}