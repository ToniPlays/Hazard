#pragma once

#include "Hazard/Core/Core.h"
#include "Shaders/Shader.h"
#include "Buffers/VertexArray.h"
#include "Hazard/Modules/Renderer/Buffers/Buffer.h"
#include "Hazard/Modules/Renderer/Shaders/Shader.h"
#include "Hazard/Modules/Renderer/Textures/Texture2D.h"
#include "Hazard/Modules/Renderer/Textures/RenderTexture.h"


namespace Hazard {

	enum class RenderAPI {
		None = 0, OpenGL, Vulkan, DirectX, Metal
	};
	enum class RenderType {
		None = 0, Default, Points, Wireframe
	};

	class HAZARD_API RendererAPI {
	public:
		RendererAPI(RenderAPI api) { renderer = api; }
		template<typename T>
		static T* Create();

		template<typename T>
		static T* Create(const char* file);
		static VertexBuffer* VertexBuffer(ShaderDataType type, const std::string& name);

		static RenderAPI GetAPI() { return renderer; }
		RenderType GetType() { return type; }

		int GetTypeAsInt();
		void SetType(int t);

	private:
		RenderType renderType = RenderType::Default;
		static RenderAPI renderer;
		RenderType type = RenderType::Default;
	};
}