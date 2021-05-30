#pragma once

#include "Hazard/Rendering/PostProcessing/Vignette.h"
#include "Hazard/Rendering/Pipeline/GraphicsPipeline.h"
#include "../Buffer/OpenGLVertexArray.h"
#include "../Pipeline/OpenGLFrameBuffer.h"

namespace Hazard::Rendering::OpenGL 
{
	class OpenGLVignette : public VignetteEffect
	{
	public:
		OpenGLVignette(VignetteEffectCreateInfo* info);
		~OpenGLVignette();

		FrameBuffer* Process(FrameBuffer* source, VertexArray* vao) override;
		FrameBuffer& GetTargetTexture() override { return *m_Target; }
		void UpdateData(PostProcessData& data) override;

	private:
		OpenGLFrameBuffer* m_Target;
		Ref<GraphicsPipeline> m_Pipeline;
	};
}