
#include <hzrpch.h>
#include "Pipeline.h"
#include "../RenderContextCommand.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"
#include "Platform/Rendering/Metal/Metal.h"
#include "../RenderLibrary.h"

namespace Hazard::Rendering 
{
	Ref<Pipeline> Pipeline::Create(PipelineSpecification* specs)
	{
		HZR_CORE_ASSERT(!specs->DebugName.empty(), "Unable to create pipeline with no name");

		if (specs->IsShared && RenderLibrary::HasPipeline(specs->DebugName)) 
		{
			return RenderLibrary::GetPipeline(specs->DebugName);
		}

		Ref<Pipeline> pipeline;

		switch (RenderContextCommand::GetRenderAPI())
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

		if (specs->IsShared) {
			RenderLibrary::AddPipeline(pipeline);
		}

		pipeline->m_Type = ResourceType::Pipeline;
		return pipeline;
	}
}
