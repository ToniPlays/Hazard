#pragma once

#include "Backend/Core/Texture.h"
#ifdef HZR_INCLUDE_OPENGL

namespace HazardRenderer::OpenGL 
{
	class OpenGLCubemapTexture : public CubemapTexture {
	public:
		OpenGLCubemapTexture() = default;
		OpenGLCubemapTexture(CubemapTextureCreateInfo* createInfo);

		void Bind(uint32_t slot = 0) const;

		ImageFormat GetFormat() const override {	return m_Format; };
		uint32_t GetWidth() const override { return m_Width; };
		uint32_t GetHeight() const override { return m_Height; }
		glm::uvec2 GetSize() const override { return { m_Width, m_Height }; };
		uint32_t GetMipLevels() const override { return 1; }
		const std::string& GetPath() const { return m_FilePath;	};

		Ref<Image2D> GetSourceImage() { return m_SourceImage; }

		const uint32_t& GetID() const { return m_ID;	};

	private:
		void GenerateFromCubemap(CubemapGen& generationData);

	private:
		ImageFormat m_Format = ImageFormat::None;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_ID = 0;

		std::string m_FilePath = "";
		std::string m_DebugName = "";

		Ref<Image2D> m_SourceImage = nullptr;
	};
}
#endif
