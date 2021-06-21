#pragma once

#include "Hazard/Rendering/Pipeline/FrameBuffer.h"
#include "Hazard/Rendering/Buffers/VertexArray.h"

namespace Hazard::Rendering 
{
	struct PostProcessData;

	struct VignetteEffectCreateInfo 
	{
		bool use = true;
		float intensity = 0.0f;
		float outer = 0.0f;
		float inner = 0.0f;
	};

	struct GaussianBlurCreateInfo 
	{
		bool use = false;
	};
	struct BloomCreateInfo {
		bool use = true;
		float threshold = 0.0f;
		float intensity = 0.0f;
	};

	class PostProcessingEffect 
	{
	public:
		virtual FrameBuffer* Process(FrameBuffer* source, VertexArray* vao) = 0;
		virtual void UpdateData(PostProcessData& data) = 0;
		virtual FrameBuffer& GetTargetTexture() = 0;

	public:
		bool use = false;
	};
}