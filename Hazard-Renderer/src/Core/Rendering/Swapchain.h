#pragma once

#include "Ref.h"
#include "RenderPass.h"
#include "FrameBuffer.h"
#include <Core/Rendering/RenderCommandBuffer.h>

namespace HazardRenderer 
{
	class Window;

	class Swapchain : public RefCount 
	{
	public:
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void BeginFrame() = 0;
		virtual void Present() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual Ref<RenderCommandBuffer> GetSwapchainBuffer() = 0;
		virtual Ref<RenderPass> GetRenderPass() = 0;
		virtual Ref<FrameBuffer> GetRenderTarget() = 0;
	};
}
