#pragma once

#include "VertexData.h"
#include "../Material/Material.h"

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

	class Mesh {
	public:
		Mesh() = default;
		Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
		~Mesh();

		void Render();

		Material& GetMaterial() { return *material; }
		void SetMaterial(Material* material) { this->material = material; }
		void GenerateArrays();

	private:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		std::vector<SubMesh> submeshes;
		Material* material;

		VertexArray* meshVAO;
	};
}