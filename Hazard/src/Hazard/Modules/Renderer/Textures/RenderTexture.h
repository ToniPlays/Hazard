#pragma once

#include "Texture2D.h"

namespace Hazard {
	
	class HAZARD_API RenderTexture : public Texture2D {
	public:

		RenderTexture() : Texture2D("RENDER_TEXTURE") {}
		~RenderTexture() {};

		virtual void SetWidth(unsigned int w) { width = w; };
		virtual void SetHeight(unsigned int h) { height = h; };

		virtual uint32_t GetColorID() const = 0;
	};
}