
#include "RenderContextCreateInfo.h"

namespace HazardRenderer
{
	std::string RenderAPIToString(RenderAPI api)
	{
		switch (api) {
		case RenderAPI::Auto:		return "Auto";
		case RenderAPI::OpenGL:		return "OpenGL";
		case RenderAPI::Vulkan:		return "Vulkan";
		case RenderAPI::DX11:		return "DX11";
		case RenderAPI::DX12:		return "DX12";
		case RenderAPI::Metal:		return "Metal";
		default:					return "Auto";
		}
	}
}
