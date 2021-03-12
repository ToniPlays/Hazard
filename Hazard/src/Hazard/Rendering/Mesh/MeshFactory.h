#pragma once

#include "Mesh.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Hazard::Rendering {
	class MeshFactory {
	public:
		static Mesh* LoadMesh(const char* file);
		static Mesh* LoadCube();

	private:
		static Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};
}