#pragma once

#include "Hazard/Rendering/Pipeline/FrameBuffer.h"

namespace Hazard::Rendering::OpenGL {
	class OpenGLRenderPass : public RenderPass {
	public:
		OpenGLRenderPass(RenderPassCreateInfo* info);
		~OpenGLRenderPass();

		virtual RenderPassSpecification GetSpecs() { return m_Specs; };
		virtual const RenderPassSpecification GetSpecs() const { return m_Specs; };

	private:
		RenderPassSpecification m_Specs;
	};
}