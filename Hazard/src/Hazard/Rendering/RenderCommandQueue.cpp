#pragma once
#include "hzrpch.h"
#include "RenderCommandQueue.h"

namespace Hazard::Rendering {

	RenderCommandQueue::RenderCommandQueue()
	{
		m_CommandBuffer = new uint8_t[10 * 1024 * 1024];
		m_CommandBufferPtr = m_CommandBuffer;
		memset(m_CommandBuffer, 0, 10 * 1024 * 1024);
	}
	RenderCommandQueue::~RenderCommandQueue()
	{
		delete[] m_CommandBuffer;
	}
	void* RenderCommandQueue::Allocate(RenderCommandFn func, uint32_t size)
	{
		*(RenderCommandFn*)m_CommandBufferPtr = func;
		m_CommandBufferPtr += sizeof(RenderCommandFn);

		*(uint32_t*)m_CommandBufferPtr = size;
		m_CommandBufferPtr += sizeof(uint32_t);

		void* memory = m_CommandBufferPtr;
		m_CommandBufferPtr += size;

		m_CommandCount++;
		return memory;
	}
	void RenderCommandQueue::Excecute()
	{
		byte* buffer = m_CommandBuffer;

		for (uint32_t i = 0; i < m_CommandCount; i++) {
			RenderCommandFn func = *(RenderCommandFn*)buffer;
			buffer += sizeof(RenderCommandFn);

			uint32_t size = *(uint32_t*)buffer;
			buffer += sizeof(uint32_t);
			func(buffer);
			buffer += size;
		}
	}
}
