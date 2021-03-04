#pragma once

#include "Texture2D.h"

namespace Hazard::Rendering {

	struct RenderTextureSpec {
		uint32_t width = 0, height = 0;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class HAZARD_API RenderTexture : public Texture2D {
	public:

		RenderTexture(const char* file) : Texture2D(file) {}
		~RenderTexture() {};

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual RenderTextureSpec& GetSpec() = 0;
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual uint32_t GetColorID() const = 0;
	};
}