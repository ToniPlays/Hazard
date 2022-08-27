
#include "Shader.h"
#include "Backend/Core/GraphicsContext.h"

#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/MetalCore.h"

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
		//case RenderAPI::Vulkan: return Ref<Vulkan::VulkanShader>::Create(path);
#endif
#ifdef HZR_INCLUDE_METAL
        case RenderAPI::Metal: return Ref<Metal::MetalShader>::Create(path);
#endif
		default:
			HZR_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
		return nullptr;
    }
}
