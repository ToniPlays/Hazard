#pragma once

#include "Hazard/Assets/Asset.h"
#include "Hazard/Core/Core.h"
#include "HazardRendererCore.h"
#include <glm/glm.hpp>

namespace Hazard {

	struct Vertex3D
	{
		glm::vec3 Position = { 0, 0, 0 };
		glm::vec4 Color = Color::White;
		glm::vec3 Normals = { 0, 0, 0 };
		glm::vec3 Tangent = { 0, 0, 0 };
		glm::vec3 Binormal = { 0, 0, 0 };
		glm::vec2 TexCoords = { 0, 0 };

		static HazardRenderer::BufferLayout Layout() {
			using namespace HazardRenderer;
			return { 
				{ "a_Position",			ShaderDataType::Float3 },
				{ "a_Color",			ShaderDataType::Float4 },
				{ "a_Normal",			ShaderDataType::Float3 },
				{ "a_Tangent",			ShaderDataType::Float3 },
				{ "a_Binormal",			ShaderDataType::Float3 },
				{ "a_TextureCoords",	ShaderDataType::Float2 }
			};
		}
	};

	struct SubMesh {

		uint32_t BaseVertex;
		uint32_t BaseIndex;
		uint32_t MaterialIndex;
		uint32_t IndexCount;
		uint32_t VertexCount;

		glm::mat4 Transform;
		glm::mat4 LocalTransform;

		std::string NodeName, MeshName;
	};

	class Mesh : public Asset 
	{
	public:
		Mesh() = default;
		Mesh(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices);
		Mesh(Ref<HazardRenderer::VertexBuffer> vertexBuffer, Ref<HazardRenderer::IndexBuffer> indexBuffer, Ref<HazardRenderer::Pipeline> pipeline);
		~Mesh() = default;

		bool IsValid() { return m_VertexBuffer && m_IndexBuffer; }

		size_t GetIndexCount() { return m_IndexBuffer->GetCount(); }

		Ref<HazardRenderer::VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
		Ref<HazardRenderer::IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
		Ref<HazardRenderer::Pipeline> GetPipeline() { return m_Pipeline; }

	private:
		Ref<HazardRenderer::VertexBuffer> m_VertexBuffer = nullptr;
		Ref<HazardRenderer::IndexBuffer> m_IndexBuffer = nullptr;
		Ref<HazardRenderer::Pipeline> m_Pipeline = nullptr;

		Buffer m_LocalVertexData;
		Buffer m_LocalIndexData;
	};
}