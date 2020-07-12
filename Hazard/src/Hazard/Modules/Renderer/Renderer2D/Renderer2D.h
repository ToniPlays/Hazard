#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/ECS/Components/Mesh.h"

namespace Hazard {
	class HAZARD_API Renderer2D {
	public:
		Renderer2D();
		~Renderer2D();

		void Render();
		void OnResized(Event& e);

		static int size;

	private:
		void BeginScene();
		void EndScene();

	};
}