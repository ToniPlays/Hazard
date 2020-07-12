#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/ECS/ECS.h"

namespace Hazard {

	class HAZARD_API SpriteRenderer : public Component {
	public:
		SpriteRenderer(GameObject* gameObject, Mesh* mesh);
		~SpriteRenderer();

		Mesh& GetMesh() { return *mesh; }

	private:
		Mesh* mesh;
	};
}