#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Textures/Texture2D.h"
#include <iostream>
#include "glm/glm.hpp"

namespace Hazard::Rendering::OpenGL {

	class OpenGLTexture2D : public Texture2D {
	public:

		OpenGLTexture2D(const char* file, TextureSpecs params);
		~OpenGLTexture2D();

		void Bind(uint32_t slot) const override;
		void Unbind(uint32_t slot) const override;

		virtual uint32_t GetWidth() const { return m_Spec.width; }
		virtual uint32_t GetHeight() const { return m_Spec.height; }
		virtual void SetData(void* data, uint32_t size) override;

		uint32_t GetID() const { return m_TextureID; };
		TextureSpecs& GetSpec() { return m_Spec; };

	private:
		TextureSpecs m_Spec;
		uint32_t m_TextureID;

		uint32_t m_DataFormat, m_InternalFormat;
	};
}