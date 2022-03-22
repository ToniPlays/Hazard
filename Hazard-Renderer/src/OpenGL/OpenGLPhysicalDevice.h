#pragma once

#include "Core/Core.h"
#include "Core/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace HazardRenderer::OpenGL {

	class OpenGLPhysicalDevice : public PhysicalDevice {
	public:
		OpenGLPhysicalDevice();

		virtual ~OpenGLPhysicalDevice() {};
		virtual std::string GetDeviceName() { return m_DeviceName; }
		virtual const PhysicalDeviceLimits& GetDeviceLimits() const { return m_Limits; };

		static Ref<PhysicalDevice> Create();

	private:
		std::string m_DeviceName;
		PhysicalDeviceLimits m_Limits;
	};
}