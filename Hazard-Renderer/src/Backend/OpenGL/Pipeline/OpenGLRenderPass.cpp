
#include "OpenGLRenderPass.h"
#ifdef HZR_INCLUDE_OPENGL

namespace HazardRenderer::OpenGL
{
	OpenGLRenderPass::OpenGLRenderPass(RenderPassCreateInfo* info)
	{
		m_Specs.DebugName = info->DebugName;
		m_Specs.TargetFrameBuffer = info->pTargetFrameBuffer;
	}
	OpenGLRenderPass::~OpenGLRenderPass()
	{

	}
}

#endif
