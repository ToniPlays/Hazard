#pragma once

#include "Texture2D.h"

namespace Hazard::Rendering {

	struct RenderTextureCreateInfo {

		const char* name = "RenderTexture";
		uint32_t width = 0;
		uint32_t height = 0;
		TextureDataType datatype = TextureDataType::Auto;
	};
	struct RenderTextureInfo {
		uint32_t colorID = 0;
	};

	class RenderTexture : public Texture {
	public:

		RenderTexture() {}
		~RenderTexture() {};

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual RenderTextureInfo GetInfo() const = 0;
	};
}