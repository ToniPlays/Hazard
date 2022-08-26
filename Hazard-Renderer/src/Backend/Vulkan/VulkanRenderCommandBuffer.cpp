
#include "VulkanRenderCommandBuffer.h"
#ifdef HZR_INCLUDE_VULKAN
#include "VulkanContext.h"

namespace HazardRenderer::Vulkan
{
	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(uint32_t size, const std::string& name)
	{

	}
	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(const std::string& name, bool swapchain) : m_DebugName(std::move(name)), m_OwnedBySwapchain(swapchain)
	{

	}
	VulkanRenderCommandBuffer::~VulkanRenderCommandBuffer()
	{

	}
	void VulkanRenderCommandBuffer::Begin()
	{

	}
	void VulkanRenderCommandBuffer::End()
	{

	}
	void VulkanRenderCommandBuffer::Submit()
	{

	}
}
#endif