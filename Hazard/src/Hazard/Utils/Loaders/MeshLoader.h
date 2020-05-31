#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/ECS/Components/Mesh.h"

namespace Hazard {
	class HAZARD_API MeshLoader {
	public:
		static Mesh* LoadMesh(std::string file);
	private:
		static Mesh* LoadObj(std::string file);
	};
}