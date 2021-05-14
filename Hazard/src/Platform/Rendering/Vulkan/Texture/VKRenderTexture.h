#pragma once

#include "Hazard/Rendering/Textures/RenderTexture.h"

namespace Hazard::Rendering::Vulkan {

	class VKRenderTexture : public RenderTexture {
	public:

		VKRenderTexture(TextureSpecs params = TextureSpecs());
		virtual ~VKRenderTexture();

		void Invalidate();

		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot = 0) const override;

		void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetWidth() const override { return m_Spec.width; }
		virtual uint32_t GetHeight() const override { return m_Spec.height; }

		uint32_t GetColorID() const { return 0; }
		uint32_t GetID() const override { return 0; }

		TextureSpecs& GetSpec() override { return m_Spec; }
		virtual void SetData(void* data, uint32_t size) {}

	private:
		TextureSpecs m_Spec;
	};
}