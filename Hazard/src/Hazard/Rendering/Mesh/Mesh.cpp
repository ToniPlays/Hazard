#include "Mesh.h"

#include <hzrpch.h>
#include "Mesh.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "HazardRendererCore.h"

namespace Hazard
{
	using namespace HazardRenderer;

	Mesh::Mesh(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices)
	{
		m_LocalVertexData = Buffer::Copy(vertices.data(), vertices.size() * sizeof(Vertex3D));
		m_LocalIndexData = Buffer::Copy(indices.data(), indices.size() * sizeof(uint32_t));

		BufferLayout layout = Vertex3D::Layout();

		VertexBufferCreateInfo vboInfo = {};
		vboInfo.DebugName = "SomeMeshVertexBuffer";
		vboInfo.Layout = &layout;
		vboInfo.Size = m_LocalVertexData.Size;
		vboInfo.Data = m_LocalVertexData.Data;
		vboInfo.Usage = BufferUsage::StaticDraw;

		m_VertexBuffer = VertexBuffer::Create(&vboInfo);

		IndexBufferCreateInfo iboInfo = {};
		iboInfo.DebugName = "SomeMeshIndices";
		iboInfo.Size = m_LocalIndexData.Size;
		iboInfo.Data = (uint32_t*)m_LocalIndexData.Data;
		iboInfo.Usage = BufferUsage::StaticDraw;

		m_IndexBuffer = IndexBuffer::Create(&iboInfo);

		PipelineSpecification pipelineSpecs = {};
		pipelineSpecs.DebugName = "PBRShader";
		pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
		pipelineSpecs.DrawType = DrawType::Fill;
		pipelineSpecs.ShaderPath = "res/Shaders/pbr.glsl";
		pipelineSpecs.pTargetRenderPass = Application::GetModule<RenderEngine>().GetRenderPass();
		pipelineSpecs.pBufferLayout = &layout;
		pipelineSpecs.DepthTest = true;

		m_Pipeline = Pipeline::Create(&pipelineSpecs);

		Renderer::SubmitResourceFree([&]() mutable {
			m_LocalVertexData.Release();
			m_LocalIndexData.Release();
			});
	}

	Mesh::Mesh(Ref<HazardRenderer::VertexBuffer> vertexBuffer, Ref<HazardRenderer::IndexBuffer> indexBuffer, Ref<HazardRenderer::Pipeline> pipeline) : m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), m_Pipeline(pipeline)
	{
		if (pipeline == nullptr) {

			BufferLayout layout = Vertex3D::Layout();

			PipelineSpecification pipelineSpecs = {};
			pipelineSpecs.DebugName = "PBRShader";
			pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
			pipelineSpecs.DrawType = DrawType::Fill;
			pipelineSpecs.ShaderPath = "Shaders/pbr.glsl";
			//pipelineSpecs.pTargetRenderPass = Application::GetModule<RenderEngine>().GetWindow().GetSwapchain()->GetRenderPass().Raw();
			pipelineSpecs.pBufferLayout = &layout;

			m_Pipeline = Pipeline::Create(&pipelineSpecs);
		}
	}
}

