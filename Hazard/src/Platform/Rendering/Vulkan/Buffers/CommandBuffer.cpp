#pragma once
#include <hzrpch.h>
#include "CommandBuffer.h"

namespace Hazard::Rendering::Vulkan 
{
	CommandBuffer::CommandBuffer(VkDevice device, VkCommandPool pool)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = pool;
		allocInfo.commandBufferCount = 1;

		auto result = vkAllocateCommandBuffers(device, &allocInfo, &m_CommandBuffer);
		HZR_ASSERT(result == VK_SUCCESS, "Failed to create CommandBuffer");
	}
	CommandBuffer::~CommandBuffer()
	{

	}
	void CommandBuffer::Begin()
	{
		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		
		vkBeginCommandBuffer(m_CommandBuffer, &info);
	}
	void CommandBuffer::End()
	{
		vkEndCommandBuffer(m_CommandBuffer);
	}
}