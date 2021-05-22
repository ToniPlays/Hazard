#pragma once

#include "Hazard/Rendering/PostProcessing/BloomEffect.h"
#include "Hazard/Rendering/Pipeline/GraphicsPipeline.h"
#include "../Pipeline/OpenGLFrameBuffer.h"

namespace Hazard::Rendering::OpenGL
{
	class OpenGLBloom : public BloomEffect
	{
	public:
		OpenGLBloom(BloomCreateInfo* info);
		~OpenGLBloom();

		FrameBuffer* Process(FrameBuffer* source, VertexArray* vao) override;
		FrameBuffer& GetTargetTexture() override { return *m_Target; }
		void UpdateData(PostProcessData& data) override;

	private:
		OpenGLFrameBuffer* m_Target;
		GraphicsPipeline* m_Pipeline;
		GraphicsPipeline* m_CombinePipeline;
	};
}