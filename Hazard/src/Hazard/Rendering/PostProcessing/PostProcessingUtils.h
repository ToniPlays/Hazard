#pragma once

#include "Hazard/Rendering/RenderUtils.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"

namespace Hazard::Rendering 
{
	class PostProcessingUtils 
	{
	public:
		template<typename T, typename Arg>
		static T* Create(Arg args) {
			static_assert(false);
		}

		template<>
		static VignetteEffect* Create(VignetteEffectCreateInfo* info) {
			switch (RenderUtils::GetRenderAPI())
			{
			case RenderAPI::OpenGL:	return new OpenGL::OpenGLVignette(info);
			}
		}
		template<>
		static GaussianBlurEffect* Create(GaussianBlurCreateInfo* info) {
			switch (RenderUtils::GetRenderAPI())
			{
			case RenderAPI::OpenGL:	return new OpenGL::OpenGLGaussianBlur(info);
			}
		}
		template<>
		static BloomEffect* Create(BloomCreateInfo* info) {
			switch (RenderUtils::GetRenderAPI())
			{
			case RenderAPI::OpenGL:	return new OpenGL::OpenGLBloom(info);
			}
		}
	};
}