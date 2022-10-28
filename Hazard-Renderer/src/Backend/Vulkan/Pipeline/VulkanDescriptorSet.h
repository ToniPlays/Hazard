#pragma once

#include "vulkan/vulkan.h"

#include <unordered_map>
#include <vector>

namespace HazardRenderer::Vulkan
{
	class VulkanDescriptorSet
	{
	public:
		VulkanDescriptorSet() = default;

		VkDescriptorSet& GetVulkanDescriptorSet() { return m_VkDescriptorSet; }
		VkDescriptorSetLayout& GetLayout() { return m_DescriptorSetLayout; }

		void SetBuffer(uint32_t binding, VkDescriptorBufferInfo info);
		void SetSampler(VkDescriptorType type, VkSampler sampler);

		void ReserveBindings(size_t size)
		{
			m_Bindings.reserve(size);
		}
		void AddBinding(VkDescriptorSetLayoutBinding binding)
		{
			m_Bindings.push_back(binding);
		}
		void AddWriteDescriptor(uint32_t binding, VkWriteDescriptorSet descriptor)
		{
			m_WriteDescriptors[binding] = descriptor;
		}

		void Invalidate();

	private:
		VkDescriptorSet m_VkDescriptorSet;
		std::unordered_map<uint32_t, VkWriteDescriptorSet> m_WriteDescriptors;
		std::vector<VkDescriptorSetLayoutBinding> m_Bindings;
		VkDescriptorSetLayout m_DescriptorSetLayout;
	};
}