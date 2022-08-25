
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
		m_Width = createInfo->Width;
		m_Height = createInfo->Height;

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

		int w, h;

		if (createInfo->Data.Data == nullptr && !createInfo->FilePath.empty())
		{
			Buffer buffer = GenerateFromFile(w, h);
			GenerateFromData(buffer, w, h);
		}
		else GenerateFromData(createInfo->Data, 0, 0);
	}
	void OpenGLCubemapTexture::Bind(uint32_t slot) const
	{
		glBindTexture(slot, m_ID);
	}
	Buffer OpenGLCubemapTexture::GenerateFromFile(int& width, int& height)
	{
		HZR_ASSERT(File::Exists(m_FilePath), "File does not exist");

		constexpr int desired = 4;

		int channels;

		stbi_set_flip_vertically_on_load(false);

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

		//glBindImageTexture(unit, texture, level, layered, layer, access, format);
		glBindImageTexture(0, m_ID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);

		Renderer::Submit([image = m_SourceImage, pipeline]() mutable {
			pipeline->GetShader()->Set("u_EquirectangularTexture", 0, image);
			});
		pipeline->Execute(cmdBuffer);

		Renderer::SubmitResourceFree([pipeline, data = imageData]() mutable {
			data.Release();
			});
	}
}