#pragma once

#include "Hazard/Rendering/Textures/RenderTexture.h"

namespace Hazard::Rendering::Vulkan {

	class VKRenderTexture : public RenderTexture {
	public:

		VKRenderTexture(const RenderTextureCreateInfo& info);
		virtual ~VKRenderTexture();

		void Invalidate();

		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot = 0) const override;

		void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetWidth() const override { return 0; }
		virtual uint32_t GetHeight() const override { return 0; }

		RenderTextureInfo GetInfo() const override { return m_Info; }

		uint32_t GetID() const override { return 0; };
	private:
		RenderTextureInfo m_Info;
	};
}