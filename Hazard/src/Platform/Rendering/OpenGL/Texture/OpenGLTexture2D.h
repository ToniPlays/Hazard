#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Textures/Texture2D.h"
#include <iostream>
#include "glm/glm.hpp"

namespace Hazard::Rendering {

	class HAZARD_API OpenGLTexture2D : public Texture2D {
	public:

		OpenGLTexture2D(const char* file);
		OpenGLTexture2D(const glm::vec2 size, const char* name);
		~OpenGLTexture2D();

		void Bind(uint32_t slot) const override;
		void Unbind(uint32_t slot) const override;

		virtual uint32_t GetWidth() const { return width; }
		virtual uint32_t GetHeight() const { return height; }
		virtual void SetData(void* data, uint32_t size) override;

		uint32_t GetID() const { return textureID; };

	private:
		uint32_t width, height;
		uint32_t textureID;

		uint32_t dataFormat, internalFormat;
	};
}