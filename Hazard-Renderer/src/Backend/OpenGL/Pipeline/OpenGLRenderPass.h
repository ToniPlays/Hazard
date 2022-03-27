#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL

#include "Backend/Core/FrameBuffer.h"
#include "Backend/Core/Pipeline/RenderPass.h"

namespace HazardRenderer::OpenGL 
{
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

#endif
