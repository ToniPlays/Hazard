#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace HazardRenderer::OpenGL {

	class OpenGLPhysicalDevice : public PhysicalDevice {
	public:
		OpenGLPhysicalDevice();

		virtual ~OpenGLPhysicalDevice() {};
		virtual std::string GetDeviceName() { return m_DeviceName; }
		virtual const PhysicalDeviceLimits& GetDeviceLimits() const { return m_Limits; };
	private:
		std::string m_DeviceName;
		PhysicalDeviceLimits m_Limits;
	};
}
#endif
