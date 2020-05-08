#pragma once
#include "Core/Core.h"

namespace Hazard {
	class HAZARD_API Renderer2D {
	public:
		Renderer2D();
		~Renderer2D();

		void Render();
		void RenderToTexture();
	private:
		unsigned int vertexArray, vertexBuffer, indexBuffer;
	};
}