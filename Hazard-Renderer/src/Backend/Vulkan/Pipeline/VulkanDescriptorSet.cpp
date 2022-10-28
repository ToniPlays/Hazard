
#include "VulkanDescriptorSet.h"
#include "../VulkanContext.h"

namespace HazardRenderer::Vulkan
{
    void VulkanDescriptorSet::SetBuffer(uint32_t binding, VkDescriptorBufferInfo info)
    {
        const auto& device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
        auto& writeSet = m_WriteDescriptors[binding];

        writeSet.dstSet = m_VkDescriptorSet;
        writeSet.pBufferInfo = &info;

        vkUpdateDescriptorSets(device, 1, &writeSet, 0, nullptr);
    }
    void VulkanDescriptorSet::SetSampler(uint32_t binding, uint32_t index, VkDescriptorImageInfo info)
    {
        const auto& device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
        auto& writeSet = m_WriteDescriptors[binding];
        writeSet.dstSet = m_VkDescriptorSet;
        writeSet.dstArrayElement = index;
        writeSet.pImageInfo = &info;

        vkUpdateDescriptorSets(device, 1, &writeSet, 0, nullptr);
    }
    void VulkanDescriptorSet::Invalidate()
    {
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