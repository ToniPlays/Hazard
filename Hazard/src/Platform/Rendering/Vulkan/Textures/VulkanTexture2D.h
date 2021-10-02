#pragma once

#include "Hazard/Rendering/Texture/Texture2D.h"

#include "glad/glad.h"

namespace Hazard::Rendering::Vulkan
{
	class VulkanTexture2D : public Texture2D {
	public:
		VulkanTexture2D(Texture2DCreateInfo* info);
		~VulkanTexture2D();

		void Bind(uint32_t slot = 0) const override;

		uint32_t GetWidth() override { return m_Width; };
		uint32_t GetHeight() override { return m_Height; };

		void* GetID() const override { return (void*)m_ID; }

	private:

		int m_Width;
		int m_Height;
		uint32_t m_ID = 0;
	};
}