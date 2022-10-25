
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

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glGenerateTextureMipmap(m_ID);

		if (createInfo->Data.Data == nullptr && !createInfo->FilePath.empty())
		{
			//Load from file
			int w, h;
			Buffer buffer = GenerateFromFile(w, h, createInfo->FlipOnLoad);

			Image2DCreateInfo sourceImageInfo = {};
			sourceImageInfo.DebugName = "OpenGLCubemapSource";
			sourceImageInfo.Width = w;
			sourceImageInfo.Height = h;
			sourceImageInfo.Data = buffer;
			sourceImageInfo.Format = ImageFormat::RGBA16F;
			sourceImageInfo.ClearLocalBuffer = true;

			m_SourceImage = Image2D::Create(&sourceImageInfo);

			GenerateFromEquirectangular(m_SourceImage);
		}
		else if (createInfo->pCubemapSrc)
		{
			GenerateFromCubemap(*createInfo->pCubemapSrc);
		}
		else if (createInfo->pImageSrc)
		{
			Ref<Image2D> image = createInfo->pImageSrc->pSourceImage;
			GenerateFromEquirectangular(image);
		}
		else if (createInfo->Data.Data)
		{
			/*
			for (uint8_t i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F,
					m_Width, m_Height, 0, GL_RGB, GL_FLOAT, nullptr);
			}
			*/
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
	Buffer OpenGLCubemapTexture::GenerateFromFile(int& width, int& height, bool flipOnLoad)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(File::Exists(m_FilePath), "File does not exist");

		constexpr int desired = 4;

		int channels;
		stbi_set_flip_vertically_on_load(flipOnLoad);
		if (m_Format == ImageFormat::RGBA16F)
		{
			stbi_ldr_to_hdr_gamma(1.0f);
			float* data = stbi_loadf(m_FilePath.c_str(), &width, &height, &channels, desired);
			return Buffer(data, width * height * desired);
		}
		stbi_uc* data = stbi_load(m_FilePath.c_str(), &width, &height, &channels, desired);
		return Buffer(data, width * height * desired);
	}
	void OpenGLCubemapTexture::GenerateFromEquirectangular(Ref<Image2D> sourceImage)
	{
		Ref<CubemapTexture> instance = this;

		PipelineSpecification pipelineSpec = {};
		pipelineSpec.DebugName = "EquirectangularToCubemap";
		pipelineSpec.ShaderPath = "res/Shaders/Compute/EquirectangularToCubeMap.glsl";
		pipelineSpec.Usage = PipelineUsage::ComputeBit;

		Ref<Pipeline> computePipeline = Pipeline::Create(&pipelineSpec);

		auto& commandBuffer = OpenGLContext::GetInstance().GetSwapchain()->GetSwapchainBuffer();

		auto& shader = computePipeline->GetShader();
		shader->Set("u_EquirectangularTexture", 0, sourceImage);
		shader->Set("o_CubeMap", 0, instance);

		commandBuffer->BindPipeline(computePipeline);
		commandBuffer->DispatchCompute({ m_Width / 32, m_Height / 32, 6 });
		commandBuffer->InsertMemoryBarrier(MemoryBarrierBit_All);
	}
	void OpenGLCubemapTexture::GenerateFromCubemap(CubemapGen& generationData)
	{
		HZR_ASSERT(generationData.Pipeline, "No pipeline specified for cubemap generation");
		auto& cmdBuffer = OpenGLContext::GetInstance().GetSwapchain()->GetSwapchainBuffer();

		Ref<CubemapTexture> instance = this;
		generationData.Pipeline->GetShader()->Set(generationData.OutputImageName, 0, instance);

		cmdBuffer->BindPipeline(generationData.Pipeline);
		cmdBuffer->DispatchCompute({ m_Width / 32, m_Height / 32, 6 });
		cmdBuffer->InsertMemoryBarrier(MemoryBarrierBit_All);
	}
}
#endif