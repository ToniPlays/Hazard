
#include <hzrpch.h>
#include "LineRenderer.h"
#include "HRenderer.h"


namespace Hazard
{
	LineRenderer::LineRenderer()
	{

	}
	void LineRenderer::Init()
	{
		constexpr uint32_t lineCount = 5000;
		m_Data.MaxLineCount = lineCount;
		m_Data.MaxVertices = lineCount * 2;

		m_LineBatch = Batch<LineVertex>(m_Data.MaxVertices);
	}
	void LineRenderer::BeginScene()
	{
		BeginBatch();
	}
	void LineRenderer::EndScene()
	{
		HZR_PROFILE_FUNCTION();
		Flush();
	}
	void LineRenderer::BeginBatch()
	{
		HZR_PROFILE_FUNCTION();
		m_LineBatch.Reset();
	}
	void LineRenderer::Flush()
	{
		HZR_PROFILE_FUNCTION();
		if (!m_LineBatch.GetCount()) return;

		uint64_t size = m_LineBatch.GetDataSize();
		uint64_t elements = size / sizeof(LineVertex);
		m_VertexBuffer->SetData(m_LineBatch.GetData(), size);

		HRenderer::SubmitMesh(glm::mat4(1.0f), m_VertexBuffer, m_Pipeline, m_LineBatch.GetCount());
	}
	void LineRenderer::SubmitLine(const glm::vec3& startPos, const glm::vec3 endPos, const glm::vec4& color)
	{
		HZR_PROFILE_FUNCTION();

		if (m_LineBatch.GetCount() >= m_Data.MaxVertices) {
			Flush();
			BeginScene();
		}
		LineVertex startVertex = {};
		startVertex.Position = startPos;
		startVertex.Color = color;

		m_LineBatch.Push(startVertex);

		LineVertex endVertex = {};
		endVertex.Position = endPos;
		endVertex.Color = color;

		m_LineBatch.Push(endVertex);
	}

	void LineRenderer::CreateResources()
	{
		using namespace HazardRenderer;

		BufferLayout layout = { { "a_Position",		ShaderDataType::Float3 },
								{ "a_Color",		ShaderDataType::Float4 }
		};

		if (!m_VertexBuffer) {
			VertexBufferCreateInfo vertexInfo = {};
			vertexInfo.DebugName = "RendererLineBatch";
			vertexInfo.Size = (uint32_t)m_Data.MaxVertices * sizeof(LineVertex);
			vertexInfo.Usage = BufferUsage::DynamicDraw;
			vertexInfo.Layout = &layout;
			vertexInfo.Data = nullptr;

			m_VertexBuffer = VertexBuffer::Create(&vertexInfo);
		}

		PipelineSpecification pipelineSpecs = {};
		pipelineSpecs.DebugName = "LineBatchPipeline";
		pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
		pipelineSpecs.DrawType = DrawType::Line;
		pipelineSpecs.LineWidth = 3.0f;
		pipelineSpecs.ShaderPath = "Shaders/Debug/lineShader.glsl";
		pipelineSpecs.pTargetRenderPass = m_RenderPass.Raw();
		pipelineSpecs.pBufferLayout = &layout;
		pipelineSpecs.DepthTest = true;

		m_Pipeline = Pipeline::Create(&pipelineSpecs);
	}
}


