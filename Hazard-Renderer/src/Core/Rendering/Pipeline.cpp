
#include "Pipeline.h"

#include "Core/GraphicsContext.h"
#include "OpenGL/OpenGLCore.h"
#include "Vulkan/VulkanCore.h"
#include "Metal/MetalCore.h"

namespace HazardRenderer
{
	Ref<Pipeline> Pipeline::Create(PipelineSpecification* specs)
	{
		HZR_ASSERT(!specs->DebugName.empty(), "Unable to create pipeline with no name");

		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLPipeline>::Create(specs); break;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanPipeline>::Create(specs); break;
#endif
#ifdef HZR_INCLUDE_METAL
            case RenderAPI::Metal: return Ref<Metal::MetalPipeline>::Create(specs); break;
#endif
		default:
			HZR_ASSERT(false, "Unknown RenderAPI");
		}

		return nullptr;
	}
}
