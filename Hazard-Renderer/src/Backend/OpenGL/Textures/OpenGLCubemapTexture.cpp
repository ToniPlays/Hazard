
#include "OpenGLCubemapTexture.h"
#ifdef HZR_INCLUDE_OPENGL
#include "File.h"

#include "../OpenGLContext.h"
#include "Backend/Core/Renderer.h"
#include "Backend/Core/Pipeline/Pipeline.h"
#include "../OpenGLUtils.h"

#include "vendor/stb_image.h"

namespace HazardRenderer::OpenGL
{
	OpenGLCubemapTexture::OpenGLCubemapTexture(CubemapTextureCreateInfo* createInfo) : m_FilePath(createInfo->FilePath), m_Format(createInfo->Format)
	{
		HZR_PROFILE_FUNCTION();
		m_DebugName = createInfo->DebugName;

		if (createInfo->pCubemapSrc)
		{
			m_Width = createInfo->pCubemapSrc->pCubemap->GetWidth();
			m_Height = createInfo->pCubemapSrc->pCubemap->GetWidth();
		}
		else
		{
			m_Width = createInfo->Width;
			m_Height = createInfo->Height;
		}

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

		for (uint8_t i = 0; i < 6; i++)
		{
			uint32_t format = OpenGLUtils::GetGLFormat(m_Format);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
				m_Width, m_Height, 0, GL_RGB, OpenGLUtils::GetFormatType(format), nullptr);
		}

		if (createInfo->GenerateMips)
		{
			//glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			//glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			for (uint32_t i = 0; i < 6; i++)
			{
				//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F,
				//	m_Width, m_Height, 0, GL_RGB, GL_FLOAT, nullptr);
			}
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		

		if (createInfo->pCubemapSrc)
		{
			GenerateFromCubemap(*createInfo->pCubemapSrc);
		}
		else if (createInfo->Data.Data)
		{
			for (uint8_t i = 0; i < 6; i++)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F,	m_Width, m_Height, 0, GL_RGB, GL_FLOAT, createInfo->Data.Data);
		}
	}
	void OpenGLCubemapTexture::Bind(uint32_t slot) const
	{
		HZR_PROFILE_FUNCTION();
		Renderer::Submit([s = slot, id = m_ID]() mutable {
			HZR_PROFILE_FUNCTION("OpenGLCubemapTexture::Bind(uint32_t) RT");
			glBindTextureUnit(s, id);
			});
	}
	void OpenGLCubemapTexture::GenerateFromCubemap(CubemapGen& generationData)
	{
		HZR_PROFILE_FUNCTION();
		Timer timer;
		HZR_ASSERT(generationData.Pipeline, "No pipeline specified for cubemap generation");
		auto& commandBuffer = OpenGLContext::GetInstance().GetSwapchain()->GetSwapchainBuffer();

		Ref<CubemapTexture> instance = this;
		generationData.Pipeline->GetShader()->Set(generationData.OutputImageName, 0, instance);

		DispatchComputeInfo computeInfo = {};
		computeInfo.GroupSize = { m_Width / 32, m_Height / 32, 6 };
		computeInfo.Pipeline = generationData.Pipeline;
		computeInfo.WaitForCompletion = true;

		commandBuffer->DispatchCompute(computeInfo);
	}
}
#endif