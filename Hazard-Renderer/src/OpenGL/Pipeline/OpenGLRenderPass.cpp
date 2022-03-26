
#include "OpenGLRenderPass.h"

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
