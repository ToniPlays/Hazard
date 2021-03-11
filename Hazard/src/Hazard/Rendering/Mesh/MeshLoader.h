#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>

namespace Hazard::Rendering {

	class MeshLoader {
	public:
		static Mesh* LoadMesh(const char* file);
		static Mesh* LoadCube();
	};
}