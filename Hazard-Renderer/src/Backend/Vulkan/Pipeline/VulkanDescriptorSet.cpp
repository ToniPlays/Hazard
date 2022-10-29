
#include "VulkanDescriptorSet.h"
#include "../VulkanContext.h"
#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::Vulkan
{
	void VulkanDescriptorSet::SetBuffer(uint32_t binding, VkDescriptorBufferInfo info)
	{
		const auto& device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
		auto& writeSet = m_WriteDescriptors[binding];
		writeSet.Set.dstSet = m_VkDescriptorSet;
		writeSet.Set.pBufferInfo = &info;

		vkUpdateDescriptorSets(device, 1, &writeSet.Set, 0, nullptr);
	}
	void VulkanDescriptorSet::SetSampler(uint32_t binding, uint32_t index, VkDescriptorImageInfo info)
	{
		const auto& device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
		auto& writeSet = m_WriteDescriptors[binding];
		writeSet.Set.dstSet = m_VkDescriptorSet;
		writeSet.Set.dstArrayElement = index;
		writeSet.Set.pImageInfo = &info;

		//std::cout << fmt::format("{3} Sampler {0} binding {1} index {2}", writeSet.name, binding, index, m_DebugName) << std::endl;

		vkUpdateDescriptorSets(device, 1, &writeSet.Set, 0, nullptr);
	}
	void VulkanDescriptorSet::Invalidate()
	{
		std::sort(m_Bindings.begin(), m_Bindings.end(), [](VkDescriptorSetLayoutBinding& a, VkDescriptorSetLayoutBinding& b) { return a.binding < b.binding; });

		const auto& device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = (uint32_t)m_Bindings.size();
		layoutInfo.pBindings = m_Bindings.data();

		VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_DescriptorSetLayout), "Failed to create descriptor layout");

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_DescriptorSetLayout;
		m_VkDescriptorSet = VulkanContext::GetInstance()->RT_AllocateDescriptorSet(allocInfo);
	}
}