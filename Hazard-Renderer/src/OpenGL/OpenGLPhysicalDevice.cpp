

#include "OpenGLPhysicalDevice.h"
#include <glad/glad.h>

namespace HazardRenderer::OpenGL {
	OpenGLPhysicalDevice::OpenGLPhysicalDevice()
	{
		m_DeviceName = std::string((char*)glGetString(GL_RENDERER));
	}
}