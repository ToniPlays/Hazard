#pragma once

#include "Hazard/Core/Core.h"
#include "Shaders/Shader.h"
#include "Buffers/VertexArray.h"

namespace Hazard {

	class HAZARD_API Grid {
	public:
		Grid();
		~Grid();
		void Render();

	private:
		size_t size;
		Shader* shader;
		VertexArray* vertexArray;
	};
}