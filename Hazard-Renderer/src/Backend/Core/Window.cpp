#include "Window.h"

namespace HazardRenderer 
{
	bool Window::IsRenderAPISupported(const RenderAPI& api) 
	{
		switch (api)
		{
#ifdef HZR_INCLUDE_DX11
		case RenderAPI::DX11:		return true;
#endif
#ifdef HZR_INCLUDE_DX12
		case RenderAPI::DX12:		return true;
#endif
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL:		return true;
#endif
#ifdef HZR_INCLUDE_WEBGL
		case RenderAPI::WebGL:		return true;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan:		return true;
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal:		return true;
#endif
		}
		return false;
	}
}