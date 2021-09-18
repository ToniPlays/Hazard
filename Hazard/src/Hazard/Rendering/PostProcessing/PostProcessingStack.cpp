#pragma once

#include <hzrpch.h>
#include "PostProcessingStack.h"
#include "PostProcessingUtils.h"

namespace Hazard::Rendering {

	PostProcessingStack* PostProcessingStack::s_Instance;


	PostProcessingStack::PostProcessingStack(PostProcessingStackCreateInfo info)
	{
		s_Instance = this;

		if (info.gaussianBlur)
		{
			GaussianBlurEffect* effect = PostProcessingUtils::Create<GaussianBlurEffect>(info.gaussianBlur);
			effect->UpdateData(m_PostData);
			m_Effects.push_back(effect);
		}
		if (info.bloom)
		{
			if (!Get<GaussianBlurEffect>()) {

				GaussianBlurCreateInfo gaussian;
				gaussian.use = false;

				GaussianBlurEffect* effect = PostProcessingUtils::Create<GaussianBlurEffect>(&gaussian);
				effect->UpdateData(m_PostData);
				m_Effects.push_back(effect);
			}

			BloomEffect* effect = PostProcessingUtils::Create<BloomEffect>(info.bloom);
			effect->UpdateData(m_PostData);
			m_Effects.push_back(effect);
		}

		if (info.vignette)
		{
			VignetteEffect* effect = PostProcessingUtils::Create<VignetteEffect>(info.vignette);
			effect->UpdateData(m_PostData);
			m_Effects.push_back(effect);
		}

		UniformBufferCreateInfo uniformBufferInfo;
		uniformBufferInfo.binding = 1;
		uniformBufferInfo.size = sizeof(PostProcessData);

		m_PostBuffer = RenderUtils::Create<UniformBuffer>(uniformBufferInfo);

		BufferLayout layout = { {ShaderDataType::Float2, "pos"}, { ShaderDataType::Float2, "texCoords"} };

		float vertices[] = {
			-1.0, -1.0f, 0.0f, 0.0f,
			 1.0, -1.0f, 1.0f, 0.0f,
			 1.0,  1.0f, 1.0f, 1.0f,
			-1.0,  1.0f, 0.0f, 1.0f
		};
		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexBufferCreateInfo bufferInfo;
		bufferInfo.dataStream = DataStream::DynamicDraw;
		bufferInfo.layout = &layout;
		bufferInfo.data = &vertices;
		bufferInfo.size = sizeof(vertices);

		IndexBufferCreateInfo indexInfo;
		indexInfo.dataStream = DataStream::StaticDraw;
		indexInfo.size = 6;
		indexInfo.data = indices;

		VertexArrayCreateInfo arrayInfo;
		arrayInfo.bufferInfo = &bufferInfo;
		arrayInfo.indexBufferInfo = &indexInfo;

		m_VertexArray = RenderUtils::CreateRaw<VertexArray>(arrayInfo);
	}

	void PostProcessingStack::Resize(uint32_t width, uint32_t height)
	{
		for (auto effect : m_Effects) {
			effect->GetTargetTexture().Resize(width, height);
		}
	}
	FrameBuffer* PostProcessingStack::PostProcess(FrameBuffer* source, glm::vec2 size)
	{
		HZR_PROFILE_FUNCTION();

		FrameBuffer* dest = source;
		m_PostData.resolution = size;

		for (auto effect : m_Effects) {
			effect->UpdateData(m_PostData);
		}
		
		m_PostBuffer->SetData(&m_PostData, sizeof(PostProcessData));

		for (auto effect : m_Effects) {
			if (!effect->use) 
				continue;
			dest = effect->Process(dest, m_VertexArray);
		}
		return dest;
	}
}
