#pragma once

#include "Texture2D.h"

namespace Hazard {
	
	class HAZARD_API RenderTexture : public Texture2D {
	public:

		RenderTexture() : Texture2D("RENDER_TEXTURE") {}
		~RenderTexture() {};

		virtual void SetWidth(unsigned int w) { width = w; };
		virtual void SetHeight(unsigned int h) { height = h; };
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorID() const = 0;
		virtual uint32_t GetDepthID() const = 0;

	};
}