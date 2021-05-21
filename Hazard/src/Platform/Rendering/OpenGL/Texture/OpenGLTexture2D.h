#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Textures/Texture2D.h"
#include <iostream>
#include "glm/glm.hpp"

#include <stb_image.h>

namespace Hazard::Rendering::OpenGL {

	class OpenGLTexture2D : public Texture2D {
	public:

		OpenGLTexture2D(const Texture2DCreateInfo& info);
		~OpenGLTexture2D();

		void Bind(uint32_t slot) const override;
		void Unbind(uint32_t slot) const override;

		virtual uint32_t GetWidth() const { return 0; }
		virtual uint32_t GetHeight() const { return 0; }
		virtual void SetData(void* data, uint32_t size) override;

		uint32_t GetID() const { return m_TextureID; }
		TextureInfo GetData() const override { return m_Info; };

	private:

		void CheckTransparency(stbi_uc* data);

		TextureInfo m_Info;

		uint32_t m_TextureID;
		uint32_t m_DataFormat, m_InternalFormat;
	};
}