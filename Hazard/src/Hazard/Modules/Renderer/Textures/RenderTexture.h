#pragma once

#include "Texture2D.h"

namespace Hazard {

	struct RenderTextureSpec {
		uint32_t width = 0, height = 0;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};
	
	class HAZARD_API RenderTexture : public Texture2D {
	public:

		RenderTexture() : Texture2D("RENDER_TEXTURE") {}
		~RenderTexture() {};

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual RenderTextureSpec& GetSpec() = 0;

		virtual uint32_t GetColorID() const = 0;
	};
}