#pragma once

#include "Hazard/Rendering/Textures/CubemapTexture.h"

namespace Hazard::Rendering::OpenGL {

	class OpenGLCubemapTexture : public CubemapTexture {
	public:
		OpenGLCubemapTexture(TextureSpecs& spec);
		OpenGLCubemapTexture(std::vector<std::string>& faces);
		OpenGLCubemapTexture(const std::string& name, const std::string& extension);

		~OpenGLCubemapTexture();

		uint32_t GetWidth() const { return 0; };
		uint32_t GetHeight() const { return 0; };

		void SetTexture(int side, const std::string& file);

		void Bind(uint32_t slot = 0) const;
		void Unbind(uint32_t slot = 0) const;

		TextureSpecs& GetSpec() { return m_Spec; };
		uint32_t GetID() const { return m_TextureID; };

	private:
		void SetFilters();
		void AllocateFree();

	private:
		TextureSpecs m_Spec;
		uint32_t m_TextureID;

	};
}