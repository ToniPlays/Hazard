#pragma once

#include <hzrpch.h>
#include "MeshLoader.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Hazard::Rendering {

	Mesh* MeshLoader::LoadMesh(const char* file)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		return nullptr;
	}
	Mesh* MeshLoader::LoadCube()
	{
		return nullptr;
	}
}