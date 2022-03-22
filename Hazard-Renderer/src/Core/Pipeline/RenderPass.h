#pragma once

#include "Core/FrameBuffer.h"
#include "Core/Core.h"

namespace HazardRenderer {
	class FrameBuffer;
	
	struct RenderPassCreateInfo {
		Ref<FrameBuffer> pTargetFrameBuffer = nullptr;
		std::string DebugName = "";
	};

	struct RenderPassSpecification {
		Ref<FrameBuffer> TargetFrameBuffer;
		std::string DebugName;
	};

	class RenderPass : public RefCount {
	public:
		virtual ~RenderPass() = default;

		virtual RenderPassSpecification GetSpecs() = 0;
		virtual const RenderPassSpecification GetSpecs() const = 0;

		static Ref<RenderPass> Create(RenderPassCreateInfo* info);
	};
}