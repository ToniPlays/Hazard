#pragma once

#include "Hazard/Rendering/PostProcessing/Vignette.h"
#include "Hazard/Rendering/Pipeline/GraphicsPipeline.h"
#include "../Buffer/OpenGLVertexArray.h"
#include "../Pipeline/OpenGLFrameBuffer.h"

namespace Hazard::Rendering::OpenGL {
	class OpenGLVignette : public VignetteEffect {
	public:
		OpenGLVignette();
		~OpenGLVignette();

		void Process(FrameBuffer* source, glm::vec2 res) override;
		FrameBuffer& GetTargetTexture() { return *m_Target; }

	private:
		OpenGLFrameBuffer* m_Target;
		GraphicsPipeline* m_Pipeline;

		VertexArray* m_VertexArray;
	};
}