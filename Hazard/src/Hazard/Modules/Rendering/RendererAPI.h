#pragma once

#include "Hazard/Core/Core.h"
#include "Buffers/VertexArray.h"
#include "Buffers/Buffer.h"
#include "Shaders/Shader.h"
#include "Textures/Texture2D.h"


namespace Hazard {

	enum class RenderAPI {
		None = 0, OpenGL, Vulkan, DirectX, Metal
	};
	enum class RenderType {
		Default = 0, Points
	};

	class HAZARD_API RendererAPI {
	public:

		static RenderAPI GetAPI() { return renderer; }
		static Shader* Shader(const std::string& file);
		static VertexArray* VertexArray();

		static VertexBuffer* VertexBuffer(ShaderDataType type, const std::string& name);
		static IndexBuffer* IndexBuffer();
		static Texture2D* Texture2D(const std::string& file);

		void GetType(int &selected);
		void SetType(int t);

		RenderType GetType() { return type; };

	private:
		RenderType renderType = RenderType::Default;
		static RenderAPI renderer;
		RenderType type = RenderType::Default;
	};
}