#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Core/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace HazardRenderer::OpenGL {

	class OpenGLPhysicalDevice : public PhysicalDevice {
	public:
		OpenGLPhysicalDevice();

		virtual ~OpenGLPhysicalDevice() {};
		virtual std::string GetDeviceName() { return m_DeviceName; }
		virtual const PhysicalDeviceLimits& GetDeviceLimits() const { return m_Limits; };
		virtual const PhysicalDeviceCapabilities& GetDeviceCababilities() const { return m_Capabilities; };

		virtual bool SupportsRaytracing() const { return false; };
	private:
		std::string m_DeviceName;
		PhysicalDeviceLimits m_Limits;
		PhysicalDeviceCapabilities m_Capabilities;
	};
}
#endif
