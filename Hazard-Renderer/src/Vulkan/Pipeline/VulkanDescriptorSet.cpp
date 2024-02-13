
#include "VulkanDescriptorSet.h"
#ifdef HZR_INCLUDE_VULKAN

#include "../VulkanContext.h"
#include "Core/Renderer.h"
#include "../Textures/VulkanImage2D.h"
#include "../Textures/VulkanCubemap.h"
#include "../Textures/VulkanSampler.h"
#include "VulkanGPUBuffer.h"
#include "../VKUtils.h"

#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::Vulkan
{
	VulkanDescriptorSet::VulkanDescriptorSet(DescriptorSetCreateInfo* createInfo)
	{
		m_Set = createInfo->Set;
		m_Layout = *createInfo->pLayout;
		m_DebugName = createInfo->DebugName;
		Invalidate();
	}
	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		Renderer::SubmitResourceFree([layout = m_DescriptorSetLayout]() mutable {
			const auto& device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
			vkDestroyDescriptorSetLayout(device, layout, nullptr);
		});
	}
	void VulkanDescriptorSet::Write(uint32_t binding, uint32_t index, Ref<Image> image, Ref<Sampler> sampler, bool updateAll)
	{
		Ref<VulkanDescriptorSet> instance = this;
		Ref<VulkanSampler> vkSampler = sampler.As<VulkanSampler>();

		Renderer::Submit([instance, image, vkSampler, binding, index, updateAll]() mutable {
			auto device = VulkanContext::GetLogicalDevice();
			VkDescriptorImageInfo imageDescriptor;

			if (image->GetType() == ImageType::Image2D)
				imageDescriptor = image.As<VulkanImage2D>()->GetImageDescriptor();
			else imageDescriptor = image.As<VulkanCubemap>()->GetImageDescriptor();

			if (vkSampler)
				imageDescriptor.sampler = vkSampler->GetVulkanSampler();

			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.descriptorType = VkUtils::GetDescriptorType(instance->m_Layout.GetElements()[binding].Type);
			write.dstBinding = binding;
			write.descriptorCount = 1;
			write.pImageInfo = &imageDescriptor;
			write.dstArrayElement = index;
			write.pBufferInfo = nullptr;

			if (!updateAll)
			{
				write.dstSet = instance->m_VkDescriptorSet[VulkanContext::GetFrameIndex()];
				vkUpdateDescriptorSets(device->GetVulkanDevice(), 1, &write, 0, nullptr);
			}
			else
			{
				std::vector<VkWriteDescriptorSet> writes(instance->m_VkDescriptorSet.size(), write);
				for (uint32_t i = 0; i < writes.size(); i++)
					writes[i].dstSet = instance->m_VkDescriptorSet[i];
				vkUpdateDescriptorSets(device->GetVulkanDevice(), writes.size(), writes.data(), 0, nullptr);
			}
		});
	}
	void VulkanDescriptorSet::Write(uint32_t binding, Ref<GPUBuffer> buffer, bool updateAll)
	{
		Ref<VulkanDescriptorSet> instance = this;
		Ref<VulkanGPUBuffer> vkBuffer = buffer.As<VulkanGPUBuffer>();

		Renderer::Submit([instance, vkBuffer, binding, updateAll]() mutable {
			auto device = VulkanContext::GetLogicalDevice();

			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = vkBuffer->GetVulkanBuffer();
			bufferInfo.range = vkBuffer->GetSize();
			bufferInfo.offset = 0;

			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.descriptorType = VkUtils::GetDescriptorType(instance->m_Layout.GetElements()[binding].Type);
			write.dstBinding = binding;
			write.descriptorCount = 1;
			write.pBufferInfo = &bufferInfo;
			write.dstArrayElement = 0;

			if (!updateAll)
			{
				write.dstSet = instance->m_VkDescriptorSet[VulkanContext::GetFrameIndex()];
				vkUpdateDescriptorSets(device->GetVulkanDevice(), 1, &write, 0, nullptr);
			}
			else
			{
				std::vector<VkWriteDescriptorSet> writes(instance->m_VkDescriptorSet.size(), write);
				for (uint32_t i = 0; i < writes.size(); i++)
					writes[i].dstSet = instance->m_VkDescriptorSet[i];
				vkUpdateDescriptorSets(device->GetVulkanDevice(), writes.size(), writes.data(), 0, nullptr);
			}
		});
	}
	VkDescriptorSet VulkanDescriptorSet::GetVulkanDescriptorSet()
	{
		uint32_t index = VulkanContext::GetFrameIndex();
		return m_VkDescriptorSet[index];
	}
	void VulkanDescriptorSet::Invalidate()
	{
		Ref<VulkanDescriptorSet> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
		});
	}
	void VulkanDescriptorSet::Invalidate_RT()
	{
		auto device = VulkanContext::GetLogicalDevice();

		std::vector<VkDescriptorSetLayoutBinding> bindings;

		for (auto& element : m_Layout)
		{
			auto& binding = bindings.emplace_back();
			binding.binding = element.Binding;
			binding.descriptorType = VkUtils::GetDescriptorType(element.Type);
			binding.descriptorCount = element.Length;
			binding.stageFlags = VkUtils::GetVulkanShaderStage(element.Flags);
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = bindings.size();
		layoutInfo.pBindings = bindings.data();

		VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->GetVulkanDevice(), &layoutInfo, nullptr, &m_DescriptorSetLayout), "Failed to create descriptor set layout");
		VkUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(), VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, fmt::format("VkDescriptorSetLayout {0}", m_DebugName), m_DescriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_DescriptorSetLayout;

		m_VkDescriptorSet.resize(VulkanContext::GetImagesInFlight());

		for (uint32_t i = 0; i < VulkanContext::GetImagesInFlight(); i++)
		{
			m_VkDescriptorSet[i] = VulkanContext::GetInstance()->RT_AllocateDescriptorSet(allocInfo);
			VkUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(), VK_OBJECT_TYPE_DESCRIPTOR_SET, fmt::format("VkDescriptorSet {0}", m_DebugName), m_VkDescriptorSet[i]);
		}
	}
}
#endif
