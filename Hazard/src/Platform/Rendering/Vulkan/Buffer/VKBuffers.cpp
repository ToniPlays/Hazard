#pragma once
#include <hzrpch.h>
#include "VKBuffers.h"

namespace Hazard::Rendering::Vulkan 
{
	VKVertexBuffer::VKVertexBuffer(const VertexBufferCreateInfo& info)
	{

	}
	VKVertexBuffer::~VKVertexBuffer()
	{

	}
	void VKVertexBuffer::Bind() const
	{

	}
	void VKVertexBuffer::Unbind() const
	{

	}
	void VKVertexBuffer::SetData(const void* data, uint32_t size)
	{

	}
	VKIndexBuffer::VKIndexBuffer(const IndexBufferCreateInfo& info)
	{

	}
	VKIndexBuffer::~VKIndexBuffer()
	{

	}
	void VKIndexBuffer::Bind() const
	{

	}
	void VKIndexBuffer::Unbind() const
	{

	}
	uint32_t VKIndexBuffer::GetID() const
	{
		return 0;
	}
	uint32_t VKIndexBuffer::GetCount() const
	{
		return 0;
	}
	void VKIndexBuffer::SetData(uint32_t* indices, uint32_t count)
	{

	}
}