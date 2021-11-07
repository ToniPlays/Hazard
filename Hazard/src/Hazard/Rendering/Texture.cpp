#pragma once

#include <hzrpch.h>
#include "Texture.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering {
	Ref<Texture2D> Texture2D::Create(Texture2DCreateInfo* info) {
		switch (RenderCommand::GetAPI()) {
		case RenderAPI::OpenGL: return new OpenGL::OpenGLTexture2D(info);
		//case RenderAPI::Vulkan: return new Vulkan::VulkanTexture2D(info);
		}
	}
}