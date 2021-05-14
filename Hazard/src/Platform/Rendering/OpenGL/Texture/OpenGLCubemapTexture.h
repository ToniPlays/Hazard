#pragma once

#include "Hazard/Rendering/Textures/CubemapTexture.h"

namespace Hazard::Rendering::OpenGL {

	class OpenGLCubemapTexture : public CubemapTexture {
	public:
		OpenGLCubemapTexture(const CubemapCreateInfo& info);
		~OpenGLCubemapTexture();

		uint32_t GetWidth() const { return m_Info.width; };
		uint32_t GetHeight() const { return m_Info.height; };

		void SetTexture(int side, const std::string& file);

		void Bind(uint32_t slot = 0) const;
		void Unbind(uint32_t slot = 0) const;

		uint32_t GetID() const { return m_TextureID; };
		CubemapInfo GetInfo() const { return m_Info; }

	private:
		void SetFilters();
		void AllocateFree();
		void CreateCubemapFromTexture(const std::string& file);

	private:
		CubemapInfo m_Info;
		uint32_t m_TextureID;

	};
}