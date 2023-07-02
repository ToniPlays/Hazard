#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/DescriptorSet.h"

#include "vulkan/vulkan.h"
#include <unordered_map>
#include <vector>
#include <iostream>

namespace HazardRenderer::Vulkan
{
	struct WriteDescriptorData
	{
		std::string Name;
		VkWriteDescriptorSet Set;
		VkDescriptorType Type;
	};


	class VulkanDescriptorSet : public DescriptorSet
	{
	public:
		VulkanDescriptorSet(DescriptorSetCreateInfo* createInfo);
		~VulkanDescriptorSet();

		void Write(uint32_t binding, Ref<Image> image, Ref<Sampler> sampler, bool updateAll = false) override;
		void Write(uint32_t binding, Ref<GPUBuffer> buffer, bool updateAll = false) override;

		VkDescriptorSet GetVulkanDescriptorSet();
		VkDescriptorSetLayout& GetLayout() { return m_DescriptorSetLayout; }

		void Invalidate();
		void Invalidate_RT();

	private:
		uint32_t m_Set;
		DescriptorSetLayout m_Layout;
		std::vector<VkDescriptorSet> m_VkDescriptorSet;
		VkDescriptorSetLayout m_DescriptorSetLayout;
	};
}
#endif
