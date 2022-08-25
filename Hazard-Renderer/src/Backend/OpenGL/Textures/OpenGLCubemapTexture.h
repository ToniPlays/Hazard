#pragma once

#include "Backend/Core/Texture/Texture.h"

namespace HazardRenderer::OpenGL 
{
	class OpenGLCubemapTexture : public CubemapTexture {
	public:
		OpenGLCubemapTexture() = default;
		OpenGLCubemapTexture(CubemapTextureCreateInfo* createInfo);

		void Bind(uint32_t slot = 0) const;

		ImageFormat GetFormat() const {	return m_Format; };
		uint32_t GetWidth() const { return m_Width; };
		uint32_t GetHegith() const { return m_Height; }
		glm::uvec2 GetSize() const { return { m_Width, m_Height }; };
		const std::string& GetPath() const { return m_FilePath;	};

	private:
		Buffer GenerateFromFile(int& width, int& height);
		void GenerateFromData(Buffer& imageData, int width, int height);

	private:
		ImageFormat m_Format = ImageFormat::None;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_ID = 0;

		std::string m_FilePath;

		Ref<Image2D> m_SourceImage;
	};
}