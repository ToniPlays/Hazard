#pragma once

#include "Hazard/Rendering/Texture/Texture.h"

#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan 
{
	class VulkanTexture : public Texture {
	public:
		VulkanTexture(const std::string& file);
		~VulkanTexture();

		void Bind() override;
		void Bind(VkPipelineLayout layout);
		void Unbind() override;
		void SetID(void* id) { m_ID = id; }

		VkSampler GetSampler() { return m_ImageSampler; }
		VkImageView GetView() { return m_ImageView; }
		VkImageLayout GetLayout() { return m_Layout; }

		uint32_t GetWidth() override { return m_Width; };
		uint32_t GetHeight() override { return m_Height; };

		void* GetID() const override { return m_ID; }

	private:

		VkImage m_Image;
		VkSampler m_ImageSampler;
		VkImageLayout m_Layout;
		VkImageView m_ImageView;
		VkDeviceMemory m_ImageMemory;
		VkDescriptorSetLayout m_DescriptorSetLayout;

		int m_Width;
		int m_Height;
		void* m_ID;
	};
}