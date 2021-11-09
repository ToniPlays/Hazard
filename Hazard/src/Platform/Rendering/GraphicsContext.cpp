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
		}
		HZR_CORE_ASSERT(false, "Unkonw RenderAPI");
		return nullptr;
	}
}
