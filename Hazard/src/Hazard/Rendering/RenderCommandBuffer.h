#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard::Rendering {

	struct RenderStats {
		size_t VertexCount = 0;
		size_t IndexCount = 0;
		size_t QuadCount = 0;
		size_t DrawCalls = 0;

		void Reset() {
			VertexCount = 0;
			IndexCount = 0;
			QuadCount = 0;
			DrawCalls = 0;
		}
	};

	class RenderCommandBuffer : public RefCount
	{
		friend class Pipeline;
	public:
		virtual ~RenderCommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Submit() = 0;

		virtual RenderStats& GetStats() = 0;

		static Ref<RenderCommandBuffer> Create(const std::string& debugName = "", uint32_t count = 0);
		static Ref<RenderCommandBuffer> CreateFromSwapchain(const std::string& debugName = "");
	};
}