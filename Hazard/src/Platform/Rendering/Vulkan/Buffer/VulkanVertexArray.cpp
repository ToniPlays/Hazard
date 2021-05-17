#pragma once

#include <hzrpch.h>
#include "VulkanVertexArray.h"

#include "VulkanBuffers.h"

namespace Hazard::Rendering::Vulkan {

	VulkanVertexArray::VulkanVertexArray(const VertexArrayCreateInfo& info)
	{
		m_Buffers.push_back(new VulkanVertexBuffer(*info.bufferInfo));
		m_IndexBuffer = new VulkanIndexBuffer(*info.indexBufferInfo);
	}

	VulkanVertexArray::~VulkanVertexArray()
	{

	}
	void VulkanVertexArray::Bind() const
	{

	}
	void VulkanVertexArray::Unbind() const
	{
		
	}
	void VulkanVertexArray::EnableAll(uint32_t index) const
	{

	}
}