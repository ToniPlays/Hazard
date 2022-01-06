#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Queue/RenderCommandBuffer.h"
#include "Queue/RenderCommandQueue.h"
#include "Pipeline/FrameBuffer.h"
#include "Pipeline/Buffers.h"
#include "Pipeline/Pipeline.h"
#include "Texture.h"
#include "Camera.h"

namespace Hazard::Rendering {

	class DebugRenderer;
	struct RenderPassData 
	{
		glm::mat4 ViewProjection;
	};

	class Renderer2D;

	class RenderEngine : public Module::Module
	{
		friend class RenderCommand;
	public:
		RenderEngine(RenderEngineCreateInfo* info);
		~RenderEngine();

		void Render() override;
		bool OnEvent(Event& e) override;
		void Close() override;

		template<typename Func>
		void Submit(Func&& fn) 
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (Func*)ptr;
				(*pFunc)();
			};

			auto storageBuffer = m_Queue->Allocate(renderCmd, sizeof(fn));
			new (storageBuffer) Func(std::forward<Func>(fn));
		}
		template<typename Func>
		void SubmitPostPass(Func&& fn)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (Func*)ptr;
				(*pFunc)();
			};

			auto storageBuffer = m_PostPassQueue->Allocate(renderCmd, sizeof(fn));
			new (storageBuffer) Func(std::forward<Func>(fn));
		}

		void DrawGeometry(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline);
		void DispatchPipeline(Ref<Pipeline> pipeline, uint32_t count);
		void SetLineWidth(float lineWidth);
		 
		bool OnResize(WindowResizeEvent& e);

		Renderer2D& Get2D() { return *m_Renderer2D; }
		DebugRenderer& GetDebugRenderer() { return *m_DebugRenderer; }
		Ref<RenderPass> GetCurrentRenderPass() { return m_CurrentRenderPass; }

	private:
		Renderer2D* m_Renderer2D;
		DebugRenderer* m_DebugRenderer;
		Ref<Texture2D> m_WhiteTexture;
		Ref<RenderCommandBuffer> m_RenderCommandBuffer;
		Ref<RenderPass> m_CurrentRenderPass;
		Ref<UniformBuffer> m_CameraUBO;
		RenderCommandQueue* m_PostPassQueue;
		RenderCommandQueue* m_Queue;
		RenderPassData m_RenderPassData;
	};
}