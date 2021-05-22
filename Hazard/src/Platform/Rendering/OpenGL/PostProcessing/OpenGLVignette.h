#pragma once

#include "Hazard/Rendering/PostProcessing/Vignette.h"
#include "Hazard/Rendering/Pipeline/GraphicsPipeline.h"
#include "../Buffer/OpenGLVertexArray.h"
#include "../Texture/OpenGLRenderTexture.h"

namespace Hazard::Rendering::OpenGL {
	class OpenGLVignette : public VignetteEffect {
	public:
		OpenGLVignette();
		~OpenGLVignette();

		void Process(RenderTexture* source, glm::vec2 res) override;
		RenderTexture& GetTargetTexture() { return *m_Target; }

	private:
		OpenGLRenderTexture* m_Target;
		GraphicsPipeline* m_Pipeline;

		VertexArray* m_VertexArray;
	};
}