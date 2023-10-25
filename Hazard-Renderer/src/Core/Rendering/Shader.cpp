
#include "Shader.h"
#include "Core/GraphicsContext.h"

#include "OpenGL/OpenGLCore.h"
#include "Vulkan/VulkanCore.h"
#include "Metal/MetalCore.h"

namespace HazardRenderer 
{

    Ref<Shader> Shader::Create(const std::vector<ShaderStageCode>& shaderCode)
    {
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLShader>::Create(shaderCode);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanShader>::Create(shaderCode);
#endif
#ifdef HZR_INCLUDE_METAL
        case RenderAPI::Metal: return Ref<Metal::MetalShader>::Create(shaderCode);
#endif
		default:
			HZR_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
		return nullptr;
    }
}
