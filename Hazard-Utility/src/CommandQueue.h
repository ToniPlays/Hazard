#pragma once

#include "UtilityCore.h"

class CommandQueue {
public:

	typedef void(*CommandFn)(void*);
	CommandQueue(uint32_t size = 10 * 1024 * 1024);
	~CommandQueue();

	void* Allocate(CommandFn func, uint32_t size);

	void Excecute();
	void Clear()
	{
		m_CommandBufferPtr = m_CommandBuffer;
		m_CommandCount = 0;
	};
	bool Empty() { return m_CommandCount == 0; }

private:
	uint8_t* m_CommandBuffer;
	uint8_t* m_CommandBufferPtr;
	uint32_t m_CommandCount = 0;
	uint32_t m_Size;
};
