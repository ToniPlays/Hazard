#pragma once

#include "Texture2D.h"

namespace Hazard::Rendering {

	class RenderTexture : public Texture2D {
	public:

		RenderTexture(const char* file) : Texture2D(file) {}
		~RenderTexture() {};

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual TextureSpecs& GetSpec() = 0;
		virtual uint32_t GetColorID() const = 0;
	};
}