#pragma once

#include "Hazard/Rendering/Textures/CubemapTexture.h"

namespace Hazard::Rendering::OpenGL {
	class OpenGLCubemapTexture : public CubemapTexture {
	public:
		OpenGLCubemapTexture();
		virtual ~OpenGLCubemapTexture();

		virtual unsigned int GetWidth() const;
		virtual unsigned int GetHeight() const;

		virtual void Bind(uint32_t slot = 0) const;
		virtual void Unbind(uint32_t slot = 0) const;

		virtual uint32_t GetID() const { return textureID; };

	private:
		uint32_t textureID;
	};
}