
#include "Pipeline.h"

#include "Backend/Core/GraphicsContext.h"
#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/MetalCore.h"
#include "Backend/Core/RenderLibrary.h"

namespace HazardRenderer
{
	Ref<Pipeline> Pipeline::Create(PipelineSpecification* specs)
	{
		HZR_ASSERT(!specs->DebugName.empty(), "Unable to create pipeline with no name");

		if (specs->IsShared && RenderLibrary::HasPipeline(specs->DebugName)) 
			return RenderLibrary::GetPipeline(specs->DebugName);

		Ref<Pipeline> pipeline = nullptr;

		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: pipeline = Ref<OpenGL::OpenGLPipeline>::Create(specs); break;
#endif
#ifdef HZR_INCLUDE_VULKAN
		//case RenderAPI::Vulkan: pipeline = Ref<Vulkan::VulkanPipeline>::Create(specs); break;
#endif
#ifdef HZR_INCLUDE_METAL
            case RenderAPI::Metal: pipeline = Ref<Metal::MetalPipeline>::Create(specs); break;
#endif
		default:
			return nullptr;
		}

		if (specs->IsShared)
			RenderLibrary::AddPipeline(pipeline);

		return pipeline;
	}
}
