#pragma once

#include "Scripting/CSharp/ScriptBindings.h"

namespace Hazard::Scripting::CSharp::Bindings {

	class BufferBindings {
	public:
		static BindMap GetBindings();
	private:
		static uint32_t VertexBuffer_Create_Native(void* createInfo);
		static void VertexBuffer_Destroy_Native(uint32_t resourceID);
		static uint32_t VertexBuffer_GetSize_Native(uint32_t resourceID);
	};
}