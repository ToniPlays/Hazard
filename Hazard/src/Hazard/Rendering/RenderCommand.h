#pragma once

#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Pipeline/FrameBuffer.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "RenderCommandBuffer.h"
#include "RenderEngine.h"

namespace Hazard::Rendering 
{
	class RenderCommand 
	{
		friend class RenderEngine;
	public:
		static RenderAPI GetAPI() { return s_Api; }
		//static void DrawIndexed(VertexArray* vertexArray, uint32_t size);
		template<typename T>
		static void Submit(T fn);
		static void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) { RenderContextCommand::BeginRenderPass(buffer, renderPass); }
		static void EndRenderPass(Ref<RenderCommandBuffer> buffer) { RenderContextCommand::EndRenderPass(buffer); }

		static RenderStats GetStats() { return s_Engine->GetStats(); };

	private:
		inline static RenderEngine* s_Engine;
		inline static RenderAPI s_Api;
	};
	template<typename T>
	inline void RenderCommand::Submit(T fn)
	{
		RenderContextCommand::Submit(fn);
	}
}