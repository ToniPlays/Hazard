#pragma once

#include <hzrpch.h>
#include "VKVertexArray.h"

#include "VKBuffers.h"

namespace Hazard::Rendering::Vulkan {

	VKVertexArray::VKVertexArray(const VertexArrayCreateInfo& info) 
	{
		m_Buffers.push_back(new VKVertexBuffer(*info.bufferInfo));
		m_IndexBuffer = new VKIndexBuffer(*info.indexBufferInfo);
	}

	VKVertexArray::~VKVertexArray()
	{

	}
	void VKVertexArray::Bind() const
	{

	}
	void VKVertexArray::Unbind() const
	{
		
	}
	void VKVertexArray::EnableAll(uint32_t index) const
	{

	}
}