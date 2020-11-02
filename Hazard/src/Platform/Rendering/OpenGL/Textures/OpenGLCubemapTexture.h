#pragma once
#include "Hazard/Modules/Renderer/Textures/CubemapTexture.h"

namespace Hazard {
	class OpenGLCubemapTexture : public CubemapTexture {
	public:
		OpenGLCubemapTexture(const char* name, std::vector<std::string> sources);
		virtual ~OpenGLCubemapTexture();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void Revalidate(std::vector<std::string> sources) override;

		virtual uint32_t GetID() const override { return textureID; }
	private:
		uint32_t textureID;
	};
}