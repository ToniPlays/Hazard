#pragma once

#include "Hazard/Rendering/Pipeline/FrameBuffer.h"

namespace Hazard::Rendering 
{
	class PostProcessingEffect 
	{
	public:

		virtual void Process(FrameBuffer* source) = 0;
	};
}