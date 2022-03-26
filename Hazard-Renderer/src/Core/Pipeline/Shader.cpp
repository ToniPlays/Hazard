
#include "Shader.h"
#include "Core/GraphicsContext.h"

#include "OpenGL/OpenGLCore.h"
#include "Vulkan/VulkanCore.h"
#include "Metal/Metal.h"

namespace HazardRenderer 
{

    Ref<Shader> Shader::Create(const std::string& path)
    {
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLShader>::Create(path);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanShader>::Create(path);
#endif
#ifdef HZR_INCLUDE_METAL
        case RenderAPI::Metal: return Ref<Metal::MetalShader>::Create(path);
#endif
		default:
			ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
		return nullptr;
    }
}
