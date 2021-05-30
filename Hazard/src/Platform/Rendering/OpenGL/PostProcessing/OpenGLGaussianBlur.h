#pragma once

#include "Hazard/Rendering/RenderUtils.h"
#include "Hazard/Rendering/PostProcessing/GaussianBlurEffect.h"

namespace Hazard::Rendering::OpenGL
{
	class OpenGLGaussianBlur : public GaussianBlurEffect
	{
	public:
		OpenGLGaussianBlur(GaussianBlurCreateInfo* info);
		~OpenGLGaussianBlur();

		FrameBuffer* Process(FrameBuffer* source, VertexArray* vao);
		void UpdateData(PostProcessData& data);
		FrameBuffer& GetTargetTexture() { return *m_Target; };

	private:
		FrameBuffer* m_Target;
		FrameBuffer* m_BackBuffer;

		Ref<GraphicsPipeline> m_horizontalPipeline;
		Ref<GraphicsPipeline> m_verticalPipeline;
	};
}