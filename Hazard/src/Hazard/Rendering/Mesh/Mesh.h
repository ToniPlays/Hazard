#pragma once

#include "Hazard/Assets/Asset.h"
#include "HazardRendererCore.h"
#include <glm/glm.hpp>

namespace Hazard {

	struct Vertex3D
	{
		glm::vec3 Position = { 0, 0, 0 };
		glm::vec4 Color = Color::White;
		glm::vec3 Normals = { 0, 0, 0 };
		glm::vec2 TexCoords = { 0, 0 };
	};

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


	class Mesh : public Asset {
	public:
		Mesh() = default;
		Mesh(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices);
		Mesh(Ref<HazardRenderer::VertexBuffer> vertexBuffer, Ref<HazardRenderer::IndexBuffer> indexBuffer, Ref<HazardRenderer::Pipeline> pipeline);
		~Mesh() = default;

		bool IsValid() { return m_VertexBuffer && m_IndexBuffer; }

		size_t GetIndexCount() { return m_IndexBuffer->GetCount(); }

		Ref<HazardRenderer::VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
		Ref<HazardRenderer::IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
		Ref<HazardRenderer::Pipeline> GetPipeline() { return m_Pipeline; }

	private:
		Ref<HazardRenderer::VertexBuffer> m_VertexBuffer;
		Ref<HazardRenderer::IndexBuffer> m_IndexBuffer;
		Ref<HazardRenderer::Pipeline> m_Pipeline;

		Buffer m_LocalVertexData;
		Buffer m_LocalIndexData;
	};
}