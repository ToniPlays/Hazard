#pragma once

#include "Hazard/Core/core.h"

#include "Platform/Rendering/OpenGL/Textures/OpenGLRenderTexture.h"
#include "Platform/Rendering/OpenGL/Data/OpenGLVertexArray.h"
#include "Platform/Rendering/OpenGL/Data/OpenGLBuffers.h"
#include "Platform/Rendering/OpenGL/OpenGLShader.h"


namespace Hazard {

	enum Renderer {
		Undefined = 0,
		OpenGL,
		Vulkan
	};

	class RendererHandler {

	public:
		static void Init(Renderer renderer);

		template<typename T, typename... Args>
		static T* Create(Args... args);

	private:
		static Renderer renderer;
	};
}