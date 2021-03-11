#pragma once

#include <glm/glm.hpp>

namespace Hazard::Rendering {

	struct Vertex 
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 texCoors;
	};

	class Mesh {
	public:
		Mesh();
		~Mesh();

	};
}