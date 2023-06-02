
#include "Image2D.h"
#include "Backend/Core/GraphicsContext.h"

#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/MetalCore.h"

namespace HazardRenderer 
{
    Ref<Image2D> Image2D::Create(Image2DCreateInfo* info)
    {
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLImage2D>::Create(info);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanImage2D>::Create(info);
#endif
#ifdef HZR_INCLUDE_METAL
        case RenderAPI::Metal:  return Ref<Metal::MetalImage2D>::Create(info);
#endif
		default:
			return nullptr;
		}
		return nullptr;
    }
}
