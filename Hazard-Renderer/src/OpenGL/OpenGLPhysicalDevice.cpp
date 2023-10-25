

#include "OpenGLPhysicalDevice.h"
#ifdef HZR_INCLUDE_OPENGL
#include <glad/glad.h>

namespace HazardRenderer::OpenGL 
{
	OpenGLPhysicalDevice::OpenGLPhysicalDevice()
	{
		m_DeviceName = std::string((char*)glGetString(GL_RENDERER));
	}
}
#endif
