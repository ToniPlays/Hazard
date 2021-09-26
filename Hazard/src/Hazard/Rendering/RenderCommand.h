#pragma once

#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Texture/FrameBuffer.h"
#include "Buffers/VertexArray.h"
#include "Hazard/RenderContext/RenderContextCommand.h"


namespace Hazard::Rendering {
	class RenderCommand 
	{
		friend class RenderEngine;
	public:
		static RenderAPI GetAPI() { return s_Api; }
		static void DrawIndexed(VertexArray* vertexArray, uint32_t size);
		template<typename T>
		static void Submit(T fn);

	private:
		static RenderEngine* m_Engine;

		static RenderAPI s_Api;
	};
	template<typename T>
	inline void RenderCommand::Submit(T fn)
	{
		RenderContextCommand::Submit(fn);
	}
}