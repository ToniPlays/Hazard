
#include <hzrpch.h>
#include "Shader.h"
#include "../RenderCommand.h"
#include "Hazard/Utils/StringUtil.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"
#include "Platform/Rendering/Metal/Metal.h"

namespace Hazard::Rendering 
{

    Ref<Shader> Shader::Create(const std::string& path)
    {
		HZR_PROFILE_FUNCTION();
		switch (RenderCommand::GetAPI())
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
			HZR_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
		return nullptr;
    }
}
