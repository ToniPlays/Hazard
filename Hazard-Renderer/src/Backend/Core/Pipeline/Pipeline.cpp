
#include "Pipeline.h"

#include "Backend/Core/GraphicsContext.h"
#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/MetalCore.h"

namespace HazardRenderer
{
	Ref<Pipeline> Pipeline::Create(PipelineSpecification* specs)
	{
		HZR_ASSERT(!specs->DebugName.empty(), "Unable to create pipeline with no name");

		std::cout << "Creating pipeline " << specs->DebugName << std::endl;

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
