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

	class Mesh;

	struct RenderableMesh {
		Mesh* mesh;
		glm::mat4 transform;

		RenderableMesh(Mesh* mesh, glm::mat4 transform) : mesh(mesh), transform(transform) {}
	};

	class Mesh {
	public:
		Mesh() = default;
		Mesh(std::string file, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
		~Mesh();

		void Render();


		std::string& GetFile() { return filename; }
		Material& GetMaterial() { return *material; }
		void SetMaterial(Material* material) { this->material = material; }
		void GenerateArrays();

	private:

		std::string filename;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		std::vector<SubMesh> submeshes;
		Material* material;

		VertexArray* meshVAO;
	};
}