#include "VulkanSampler.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Core/Renderer.h"
#include "../VKUtils.h"
#include "../VulkanContext.h"
#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::Vulkan
{
	VulkanSampler::VulkanSampler(SamplerCreateInfo* createInfo)
	{
		m_Info = *createInfo;
		Invalidate();
	}
	VulkanSampler::~VulkanSampler()
	{
		Renderer::SubmitResourceFree([sampler = m_Sampler]() mutable {
			const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
			vkDestroySampler(device, sampler, nullptr);
			});
	}
	void VulkanSampler::Invalidate()
	{
		Ref<VulkanSampler> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
			});
	}
	void VulkanSampler::Invalidate_RT()
	{
		HZR_PROFILE_FUNCTION();
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		if (m_Sampler)
			vkDestroySampler(device, m_Sampler, nullptr);

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.maxAnisotropy = 1.0f;

		samplerCreateInfo.magFilter = VkUtils::GetVulkanFilter(m_Info.MagFilter);
		samplerCreateInfo.minFilter = VkUtils::GetVulkanFilter(m_Info.MinFilter);
		samplerCreateInfo.mipmapMode = VkUtils::GetVulkanMipmapMode(m_Info.MinFilter);

		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU;
		samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeU;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.maxAnisotropy = 1.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = 100.0f;
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

		VK_CHECK_RESULT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &m_Sampler), "Failed to create VulkanImage2D sampler");
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SAMPLER, fmt::format("VkSampler {0}", m_Info.DebugName), m_Sampler);
	}
}
#endif