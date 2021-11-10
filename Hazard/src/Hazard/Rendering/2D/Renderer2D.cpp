#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <hzrpch.h>
#include "Renderer2D.h"
#include "Hazard/Rendering/RenderCommand.h"

namespace Hazard::Rendering
{
	Renderer2D::Renderer2D(RenderEngineCreateInfo* info)
	{
		m_RenderCommandBuffer = RenderCommandBuffer::CreateFromSwapchain("Renderer2D");

		m_Data.MaxQuadCount = info->MaxQuadCount;
		m_Data.MaxVertices = info->MaxQuadCount * 4;
		m_Data.MaxIndices = info->MaxQuadCount * 6;
		m_Data.Samplers = info->SamplerCount;

		m_Data.TextureSlots.resize(info->SamplerCount);

		uint32_t* indices = new uint32_t[m_Data.MaxIndices];
		uint32_t offset = 0;

		m_Data.QuadVertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		for (size_t i = 0; i < m_Data.MaxIndices; i += 6) {
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}
		{
			uint32_t data = 0xFFFFFFFF;
			TextureFilter filter = { FilterMode::Nearest, FilterMode::Nearest };

			Texture2DCreateInfo whiteTextureInfo = {};
			whiteTextureInfo.Width = 1;
			whiteTextureInfo.Height = 1;
			whiteTextureInfo.Data = &data;
			whiteTextureInfo.Usage = ImageUsage::Texture;
			whiteTextureInfo.Filter = &filter;
			whiteTextureInfo.Format = ImageFormat::RGBA;

			m_WhiteTexture = Texture2D::Create(&whiteTextureInfo);

			Texture2DCreateInfo starfield = {};
			starfield.FilePath = "textures/starfield_left.png";
			starfield.Usage = ImageUsage::Texture;
			starfield.Filter = &filter;

			VertexBufferCreateInfo vertexInfo = {};
			vertexInfo.Size = m_Data.MaxVertices;
			vertexInfo.Usage = BufferUsage::DynamicDraw;

			IndexBufferCreateInfo indexBuffer = {};
			indexBuffer.Data = indices;
			indexBuffer.Size = m_Data.MaxIndices;
			indexBuffer.Usage = BufferUsage::StaticDraw;

			std::vector<FrameBufferAttachment> attachments = { ImageFormat::RGBA32F, ImageFormat::Depth };

			FrameBufferCreateInfo frameBufferInfo = {};
			frameBufferInfo.SwapChainTarget = true;
			frameBufferInfo.Attachments = attachments;
			frameBufferInfo.AttachmentCount = 2;
			frameBufferInfo.Samples = 1;
			frameBufferInfo.ClearColor = { 0.1f, 0.5f, 0.5f, 1.0f };
			frameBufferInfo.DebugName = "Renderer2D FrameBuffer";

			Ref<FrameBuffer> frameBuffer = FrameBuffer::Create(&frameBufferInfo);

			RenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.DebugName = "Renderer2D";
			renderPassInfo.pTargetFrameBuffer = frameBuffer;
			Ref<RenderPass> renderPass = RenderPass::Create(&renderPassInfo);

			PipelineSpecification pipelineSpecs = {};
			pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
			pipelineSpecs.ShaderPath = "Shaders/standard.glsl";
			pipelineSpecs.pVertexBuffer = &vertexInfo;
			pipelineSpecs.pIndexBuffer = &indexBuffer;
			pipelineSpecs.RenderPass = std::move(renderPass);



			m_Pipeline = Pipeline::Create(pipelineSpecs);
			m_Data.TextureSlots[0] = m_WhiteTexture;

			Ref<Shader> shader = m_Pipeline->GetShader();
			shader->Bind();

			for (uint32_t i = 0; i < m_Data.Samplers; i++) {
				m_Data.TextureSlots[i] = m_WhiteTexture;
			}
			m_Data.TextureSlots[1] = Texture2D::Create(&starfield);

			for (uint32_t i = 0; i < m_Data.Samplers; i++) {
				shader->Set("u_Textures", i, m_Data.TextureSlots[i]);
			}
		}

		m_QuadBatch = Batch<Vertex2D>(info->MaxQuadCount);
		HZR_CORE_INFO("Quad batch size {0}", m_QuadBatch.GetSize());

		delete[] indices;
	}
	Renderer2D::~Renderer2D()
	{
		m_Pipeline.Reset();
		m_RenderCommandBuffer.Reset();
	}
	void Renderer2D::Render(const RenderPassData& renderPassData)
	{
		static float offset = 0;
		offset += Time::s_DeltaTime;

		BeginWorld();
		BeginBatch();

		float rotation = Math::Sin(Time::s_Time);

		m_Pipeline->GetShader()->SetUniformBuffer("Camera", (void*)&renderPassData);

		glm::mat4 tempMat = Math::ToTransformMatrix({ -0.4f, 0.0f, 0.2f }, { 0.0f, 0.0f,  glm::radians(rotation * 90.0f) }, { 1.0f, 1.0f, 1.0f });
		Submit({ tempMat, "#EF2E2E", 0.0f });

		tempMat = Math::ToTransformMatrix({ 0.6f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f * 1.0f, 1.0f * 1.0f, 1.0f });
		Submit({ tempMat, "#FFFFFF", 1.0f });

		Flush();
	}
	void Renderer2D::Submit(Quad quad)
	{
		if (m_QuadBatch.GetIndexCount() >= m_Data.MaxIndices)
		{
			Flush();
			BeginBatch();
		}
		for (uint8_t i = 0; i < quadVertexCount; i++)
		{
			Vertex2D vertex = {};
			vertex.Position = quad.Transform * m_Data.QuadVertexPos[i];
			vertex.Color = quad.Color;
			vertex.TextureCoords = textureCoords[i];
			vertex.TextureIndex = quad.TextureIndex;

			m_QuadBatch.Push(vertex);
		}
		m_QuadBatch.AddIndices(6);
	}
	void Renderer2D::BeginWorld()
	{
		/*for (uint32_t i = 2; i < m_Data.Samplers; i++) {
			m_Data.TextureSlots[i] = nullptr;
		}*/

		m_RenderCommandBuffer->Begin();
		RenderCommand::BeginRenderPass(m_RenderCommandBuffer, m_Pipeline->GetSpecifications().RenderPass);
	}
	void Renderer2D::BeginBatch()
	{
		m_Data.TextureIndex = 2.0f;
		m_QuadBatch.Reset();
	}
	void Renderer2D::Flush()
	{
		if (!m_QuadBatch)
			return;

		m_Pipeline->Bind();
		
		for (uint32_t i = 0; i < m_Data.TextureIndex; i++) {
			m_Data.TextureSlots[i]->Bind(i);
		}

		m_Pipeline->GetBuffer()->SetData(m_QuadBatch.GetData(), m_QuadBatch.GetDataSize());
		m_Pipeline->Draw(m_QuadBatch.GetIndexCount());

		RenderCommand::EndRenderPass(m_RenderCommandBuffer);
		m_RenderCommandBuffer->End();
		m_RenderCommandBuffer->Submit();
	}
}