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

	Mesh::Mesh(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices)
	{
		HZR_PROFILE_FUNCTION();

		BufferLayout layout = Vertex3D::Layout();

		VertexBufferCreateInfo vboInfo = {};
		vboInfo.DebugName = "SomeMeshVertexBuffer";
		vboInfo.Layout = &layout;
		vboInfo.Size = vertices.size() * sizeof(Vertex3D);
		vboInfo.Data = (void*)vertices.data();
		vboInfo.Usage = BufferUsage::StaticDraw;

		m_VertexBuffer = VertexBuffer::Create(&vboInfo);

		IndexBufferCreateInfo iboInfo = {};
		iboInfo.DebugName = "SomeMeshIndices";
		iboInfo.Size = indices.size() * sizeof(uint32_t);
		iboInfo.Data = (uint32_t*)indices.data();
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
	}

	Mesh::Mesh(Ref<HazardRenderer::VertexBuffer> vertexBuffer, Ref<HazardRenderer::IndexBuffer> indexBuffer, Ref<HazardRenderer::Pipeline> pipeline) : m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), m_Pipeline(pipeline)
	{
		HZR_PROFILE_FUNCTION();
		if (pipeline == nullptr) 
		{
			BufferLayout layout = Vertex3D::Layout();

			PipelineSpecification pipelineSpecs = {};
			pipelineSpecs.DebugName = "PBRShader";
			pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
			pipelineSpecs.DrawType = DrawType::Fill;
			pipelineSpecs.ShaderPath = "res/Shaders/pbr.glsl";
			pipelineSpecs.pBufferLayout = &layout;

			m_Pipeline = Pipeline::Create(&pipelineSpecs);
		}
	}
}

