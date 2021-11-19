#pragma once

#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Pipeline/FrameBuffer.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "RenderCommandBuffer.h"
#include "RenderEngine.h"
#include "Hazard/Entity/Component.h"

namespace Hazard::Rendering 
{
	using RenderPassCallback = std::function<void()>;

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
		static void AddRenderCallback(const RenderPassCallback& callback) { m_OnRender.push_back(callback); }

		static void DrawQuad(const ECS::SpriteRendererComponent& comp, const ECS::TransformComponent& transform);
		static void DrawRotatedQuad() {};
		static void DrawScreenSpaceQuad() {};

		static void DrawMesh() {};
		static void DrawAnimatedMesh() {};
		static void ExecuteCallbacks();

		static RenderStats GetStats() { return s_Engine->GetStats(); };
		static Ref<Texture2D> GetWhiteTexture() { return s_Engine->m_WhiteTexture; }

	private:
		inline static RenderEngine* s_Engine;
		inline static RenderAPI s_Api;
		inline static std::vector<RenderPassCallback> m_OnRender;
	};
	template<typename T>
	inline void RenderCommand::Submit(T fn)
	{
		RenderContextCommand::Submit(fn);
	}
}