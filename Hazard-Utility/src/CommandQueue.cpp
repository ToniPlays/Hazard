
#include "CommandQueue.h"

namespace HazardUtility {

	CommandQueue::CommandQueue(size_t size)
	{
		m_Size = size;
		m_CommandBuffer = new uint8_t[size];
		m_CommandBufferPtr = m_CommandBuffer;
		memset(m_CommandBuffer, 0, size);
	}
	CommandQueue::~CommandQueue()
	{
		byte* buffer = m_CommandBuffer;

		for (uint32_t i = 0; i < m_CommandCount; i++) {
			CommandFn func = *(CommandFn*)buffer;
			buffer += sizeof(CommandFn);

			uint32_t size = *(uint32_t*)buffer;
			buffer += sizeof(uint32_t);
			func.~CommandFn();
			buffer += size;
		}
		delete[] m_CommandBuffer;
	}
	void* CommandQueue::Allocate(CommandFn func, uint32_t size)
	{
		*(CommandFn*)m_CommandBufferPtr = func;
		m_CommandBufferPtr += sizeof(CommandFn);

		*(uint32_t*)m_CommandBufferPtr = size;
		m_CommandBufferPtr += sizeof(uint32_t);

		void* memory = m_CommandBufferPtr;
		m_CommandBufferPtr += size;

		m_CommandCount++;
		uint32_t dataSize = (uint32_t)((uint8_t*)m_CommandBufferPtr - (uint8_t*)m_CommandBuffer);
		ASSERT(dataSize <= m_Size, "CommandQueue overflow");
		return memory;
	}


	void CommandQueue::Excecute()
	{
		byte* buffer = m_CommandBuffer;

		for (uint32_t i = 0; i < m_CommandCount; i++) {
			CommandFn func = *(CommandFn*)buffer;
			buffer += sizeof(CommandFn);

			uint32_t size = *(uint32_t*)buffer;
			buffer += sizeof(uint32_t);
			func(buffer);
			buffer += size;
		}
	}
}
