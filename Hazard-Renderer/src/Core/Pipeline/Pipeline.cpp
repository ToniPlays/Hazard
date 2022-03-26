
#include "Pipeline.h"

#include "Core/GraphicsContext.h"
#include "OpenGL/OpenGL.h"
#include "Vulkan/VulkanCore.h"
#include "Metal/Metal.h"

namespace HazardRenderer
{
	Ref<Pipeline> Pipeline::Create(PipelineSpecification* specs)
	{
		ASSERT(!specs->DebugName.empty(), "Unable to create pipeline with no name");

		/*if (specs->IsShared && RenderLibrary::HasPipeline(specs->DebugName)) 
		{
			return RenderLibrary::GetPipeline(specs->DebugName);
		}*/
		Ref<Pipeline> pipeline = nullptr;

		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: pipeline = Ref<OpenGL::OpenGLPipeline>::Create(specs); break;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: pipeline = Ref<Vulkan::VulkanPipeline>::Create(specs); break;
#endif
#ifdef HZR_INCLUDE_METAL
            case RenderAPI::Metal: pipeline = Ref<Metal::MetalPipeline>::Create(specs); break;
#endif
		default:
			return nullptr;
		}

		//if (specs->IsShared) {
		//	RenderLibrary::AddPipeline(pipeline);
		//}

		//pipeline->m_Type = ResourceType::Pipeline;
		return pipeline;
	}
}
