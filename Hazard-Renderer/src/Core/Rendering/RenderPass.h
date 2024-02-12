#pragma once

#include "FrameBuffer.h"
#include "Core/Core.h"

namespace HazardRenderer
{
	class FrameBuffer;

	struct RenderPassCreateInfo
	{
		std::string DebugName = "";
		Ref<FrameBuffer> pTargetFrameBuffer = nullptr;
	};

	struct RenderPassSpecification
	{
		Ref<FrameBuffer> TargetFrameBuffer;
		std::string DebugName;
	};

	class RenderPass : public RefCount
	{
	public:
		virtual ~RenderPass() = default;

		virtual RenderPassSpecification GetSpecs() = 0;
		virtual const RenderPassSpecification GetSpecs() const = 0;

		static Ref<RenderPass> Create(RenderPassCreateInfo* info);
	};
}