#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/ECS/Components/Mesh.h"

namespace Hazard {
	class HAZARD_API MeshLoader {
	public:
		static Mesh* LoadMesh(std::string file);
		static Mesh* LoadCube();
		static Mesh* LoadPlane();

	private:

		static Mesh* LoadObj(std::string file);
		//OBJ
		static Vector3<float> MeshLoader::GetPosV3(std::string line);
		static Vector2<float> MeshLoader::GetPosV2(std::string line);
		static void GetData(std::string line, int& v, int& vt, int& vn);
	};
}