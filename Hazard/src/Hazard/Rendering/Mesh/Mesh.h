#pragma once

#include "VertexData.h"
#include "Hazard/Assets/Asset.h"
#include "../Pipeline/Pipeline.h"

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

	struct RenderableMesh 
	{
		Mesh* mesh;
		glm::mat4 transform;

		RenderableMesh(Mesh* mesh, glm::mat4 transform) : mesh(mesh), transform(transform) {}
	};

	class Mesh : public RuntimeResource {
		friend class MeshLoader;
	public:
		Mesh() = default;
		Mesh(const std::string& file, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices);
		Mesh(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline);
		~Mesh();

		std::string& GetFile() { return m_Filename; }
		void SetRenderPass(const Ref<RenderPass>& renderPass);
		bool IsValid() { return m_VertexBuffer && m_IndexBuffer; };

		Ref<Pipeline> GetPipeline() { return m_Pipeline; }
		Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
		Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
		size_t GetIndexCount() { return m_Indices.size(); }

		void SetPipeline(Ref<Pipeline> pipeline) { m_Pipeline = pipeline; };
		void SetVertexBuffer(Ref<VertexBuffer> vertexBuffer) { m_VertexBuffer = vertexBuffer; }
		void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) { m_IndexBuffer = indexBuffer; }

	private:
		std::string m_Filename;
		Ref<Pipeline> m_Pipeline;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		std::vector<Vertex3D> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<SubMesh> m_Submeshes;
	};
}