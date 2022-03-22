#pragma once

#include "Core/Core.h"
#include "Core/RenderContextCreateInfo.h"

namespace HazardRenderer {

	enum class State { Waiting, Record, Finished, Submit };

	class RenderCommandBuffer : public RefCount
	{
		friend class Pipeline;
	public:
		virtual ~RenderCommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Submit() = 0;

		virtual uint32_t GetFrameIndex() = 0;
		virtual bool IsRecording() = 0;

		static Ref<RenderCommandBuffer> Create(const std::string& debugName = "", uint32_t count = 0);
		static Ref<RenderCommandBuffer> CreateFromSwapchain(const std::string& debugName = "");
	};
}