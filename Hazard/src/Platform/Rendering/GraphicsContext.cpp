#include "hzrpch.h"
#include "GraphicsContext.h"
#include "Platform/System/Window.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering {

	GraphicsContext* GraphicsContext::Create(RenderAPI API, WindowProps* props)
	{
		switch (API)
		{
		case Hazard::RenderAPI::Auto:		return new OpenGL::OpenGLContext(props);
		case Hazard::RenderAPI::OpenGL:		return new OpenGL::OpenGLContext(props);
		case Hazard::RenderAPI::Vulkan:		return new Vulkan::VulkanContext(props);
		case Hazard::RenderAPI::DX11:		return nullptr;
		case Hazard::RenderAPI::DX12:		return nullptr;
		case Hazard::RenderAPI::Metal:		return nullptr;
		}
	}
}
