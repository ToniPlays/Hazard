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

		static RenderAPI GetAPI() { return renderer; }
		static Shader* Shader(const std::string& file);
		static VertexArray* VertexArray();

		static VertexBuffer* VertexBuffer(ShaderDataType type, const std::string& name);
		static IndexBuffer* IndexBuffer();
		static Texture2D* Texture2D(const std::string& file);
		static RenderTexture* RenderTexture();

		void GetType(int &selected);
		RenderType GetType() { return type; }
		void SetType(int t);

	private:
		RenderType renderType = RenderType::Default;
		static RenderAPI renderer;
		RenderType type = RenderType::Default;
	};
}