#pragma once

#include "VertexData.h"
#include "Hazard/Assets/Asset.h"

namespace Hazard::Rendering {

	struct SubMesh {

		uint32_t baseVertex;
		uint32_t baseIndex;
		uint32_t materialIndex;
		uint32_t indexCount;
		uint32_t vertexCount;

		glm::mat4 transform;
		glm::mat4 localTransform;

		std::string nodeName, meshName;
	};

	class Mesh;

	struct RenderableMesh {
		Mesh* mesh;
		glm::mat4 transform;

		RenderableMesh(Mesh* mesh, glm::mat4 transform) : mesh(mesh), transform(transform) {}
	};

	class Mesh : public Asset {
	public:
		Mesh() = default;
		Mesh(const std::string& file, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
		~Mesh();

		std::string& GetFile() { return m_Filename; }
		void GenerateArrays();
	private:
		std::string m_Filename;

		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<SubMesh> m_Submeshes;
	};
}