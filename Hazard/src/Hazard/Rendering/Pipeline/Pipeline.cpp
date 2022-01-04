
#include <hzrpch.h>
#include "Pipeline.h"
#include "../RenderCommand.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"
#include "Platform/Rendering/Metal/Metal.h"

namespace Hazard::Rendering 
{
	Ref<Pipeline> Pipeline::Create(PipelineSpecification* specs)
	{
		switch (RenderCommand::GetAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLPipeline>::Create(specs);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanPipeline>::Create(specs);
#endif
#ifdef HZR_INCLUDE_METAL
            case RenderAPI::Metal: return Ref<Metal::MetalPipeline>::Create(specs);
#endif
		default:
			HZR_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
		return nullptr;
	}
}
