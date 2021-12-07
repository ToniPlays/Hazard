#pragma once
#include "Hazard/Core/Core.h"

namespace Hazard::Rendering
{
	class RenderCommandQueue {
	public:

		typedef void(*RenderCommandFn)(void*);
		RenderCommandQueue();
		~RenderCommandQueue();

		void* Allocate(RenderCommandFn func, uint32_t size);
		void Excecute();

	private:
		uint8_t* m_CommandBuffer;
		uint8_t* m_CommandBufferPtr;
		uint32_t m_CommandCount = 0;
	};
}