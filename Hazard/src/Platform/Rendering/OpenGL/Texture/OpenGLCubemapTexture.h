#pragma once

#include "Hazard/Rendering/Textures/CubemapTexture.h"

namespace Hazard::Rendering::OpenGL {
	class OpenGLCubemapTexture : public CubemapTexture {
	public:
		OpenGLCubemapTexture();
		OpenGLCubemapTexture(const char* file);
		OpenGLCubemapTexture(std::vector<std::string>& faces);
		OpenGLCubemapTexture(const std::string& name, const std::string& extension);

		virtual ~OpenGLCubemapTexture();

		virtual unsigned int GetWidth() const;
		virtual unsigned int GetHeight() const;

		virtual void SetTexture(int side, const std::string& file);

		virtual void Bind(uint32_t slot = 0) const;
		virtual void Unbind(uint32_t slot = 0) const;

		virtual uint32_t GetID() const { return textureID; };
	private:
		void SetFilters();
	private:

		uint32_t textureID;
	};
}