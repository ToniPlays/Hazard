#pragma once


#include "GLFW/glfw3.h"
#include "vulkan/vulkan.h"
#include "WindowSurface.h"

namespace Hazard::Rendering::Vulkan 
{
	class Instance {
	public:

		Instance(GLFWwindow* window, bool enableDebugging);
		~Instance();

		static Instance& GetInstance() { return *s_Instance; }
		static WindowSurface& GetSurface() { return *s_Instance->m_Surface; }

		operator VkInstance& () { return m_Instance; }

	private:

		static Instance* s_Instance;
		GLFWwindow* m_Window;
		VkInstance m_Instance;
		WindowSurface* m_Surface;
	};
}