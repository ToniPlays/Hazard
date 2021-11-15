#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard::Rendering {
	class RenderCommandBuffer : public RefCount {
	public:
		virtual ~RenderCommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Submit() = 0;

		static Ref<RenderCommandBuffer> Create(const std::string& debugName = "", uint32_t count = 0);
		static Ref<RenderCommandBuffer> CreateFromSwapchain(const std::string& debugName = "");
	};
}