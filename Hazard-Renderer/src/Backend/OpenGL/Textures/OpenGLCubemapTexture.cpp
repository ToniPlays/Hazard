
#include "OpenGLCubemapTexture.h"
#include "File.h"

#include "../Pipeline/OpenGLComputePipeline.h"
#include "../OpenGLContext.h"
#include "Backend/Core/Renderer.h"

#include "vendor/stb_image.h"

namespace HazardRenderer::OpenGL
{
	OpenGLCubemapTexture::OpenGLCubemapTexture(CubemapTextureCreateInfo* createInfo)
	{
		m_FilePath = createInfo->FilePath;
		m_Format = createInfo->Format;

		if (createInfo->pCubemap)
		{
			m_Width = createInfo->pCubemap->GetWidth();
			m_Height = createInfo->pCubemap->GetWidth();
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
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F,
				m_Width, m_Height, 0, GL_RGB, GL_FLOAT, nullptr);
		}

		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


		if (createInfo->Data.Data == nullptr && !createInfo->FilePath.empty())
		{
			int w, h;
			Buffer buffer = GenerateFromFile(w, h);
			GenerateFromData(buffer, w, h);
		}
		else if (createInfo->pCubemap)
			GenerateFromCubemap(createInfo->pCubemap);

		else __debugbreak();
	}
	void OpenGLCubemapTexture::Bind(uint32_t slot) const
	{
		Renderer::Submit([s = slot, id = m_ID]() mutable {
			glBindTextureUnit(s, id);
			});
	}
	Buffer OpenGLCubemapTexture::GenerateFromFile(int& width, int& height)
	{
		HZR_ASSERT(File::Exists(m_FilePath), "File does not exist");

		constexpr int desired = 4;

		int channels;

		stbi_set_flip_vertically_on_load(true);

		stbi_uc* data = stbi_load(m_FilePath.c_str(), &width, &height, &channels, desired);
		HZR_ASSERT(data, "Data not loaded correctly");

		return Buffer(data, width * height * desired);
	}
	void OpenGLCubemapTexture::GenerateFromData(Buffer& imageData, int width, int height)
	{
		Image2DCreateInfo sourceImage = {};
		sourceImage.Data = imageData;
		sourceImage.Format = ImageFormat::RGBA16F;
		sourceImage.ClearLocalBuffer = true;
		sourceImage.Width = width;
		sourceImage.Height = height;
		//Generate cubemap texture

		ComputePipelineCreateInfo computeInfo = {};
		computeInfo.DebugName = "EquirectangularToCube";
		computeInfo.ShaderPath = "Shaders/Compute/EquirectangularToCubeMap.glsl";
		computeInfo.Usage = PipelineUsage::ComputeBit;

		auto& cmdBuffer = OpenGLContext::GetInstance().GetSwapchain()->GetSwapchainBuffer();

		Ref<ComputePipeline> pipeline = ComputePipeline::Create(&computeInfo);
		m_SourceImage = Image2D::Create(&sourceImage);
		m_SourceImage->Bind(cmdBuffer);
		pipeline->Bind(cmdBuffer);

		Renderer::Submit([id = m_ID]() mutable {
			//glBindImageTexture(unit, texture, level, layered, layer, access, format);
			glBindImageTexture(0, id, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
			});
		pipeline->Execute(cmdBuffer, { m_Width / 32, m_Height / 32, 6 });

		Renderer::SubmitResourceFree([pipeline, data = imageData]() mutable {
			data.Release();
			});
	}
	void OpenGLCubemapTexture::GenerateFromCubemap(Ref<CubemapTexture> cubemap)
	{
		ComputePipelineCreateInfo computeInfo = {};
		computeInfo.DebugName = "EnvironmentIrradiance";
		computeInfo.ShaderPath = "Shaders/Compute/EnvironmentIrradiance.glsl";
		computeInfo.Usage = PipelineUsage::ComputeBit;

		auto& cmdBuffer = OpenGLContext::GetInstance().GetSwapchain()->GetSwapchainBuffer();

		cubemap->Bind(1);
		Ref<ComputePipeline> pipeline = ComputePipeline::Create(&computeInfo);
		pipeline->Bind(cmdBuffer);

		Renderer::Submit([id = m_ID]() mutable {
			//glBindImageTexture(unit, texture, level, layered, layer, access, format);
			glBindImageTexture(0, id, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
			});

		pipeline->Execute(cmdBuffer, { m_Width / 32, m_Height / 32, 6 });
	}
}