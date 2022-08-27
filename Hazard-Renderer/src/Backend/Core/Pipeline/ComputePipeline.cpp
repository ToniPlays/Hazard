
#include "ComputePipeline.h"

#include "Backend/Core/GraphicsContext.h"
#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/MetalCore.h"
#include "Backend/Core/RenderLibrary.h"

namespace HazardRenderer
{
	Ref<ComputePipeline> ComputePipeline::Create(ComputePipelineCreateInfo* createInfo)
	{
		Ref<ComputePipeline> pipeline = nullptr;

		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: pipeline = Ref<OpenGL::OpenGLComputePipeline>::Create(createInfo); break;
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
		return pipeline;
	}
}
