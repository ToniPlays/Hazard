#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Queue/RenderCommandBuffer.h"
#include "Hazard/Core/CommandQueue.h"
#include "Hazard/RenderContext/Pipeline/FrameBuffer.h"
#include "Hazard/RenderContext/Pipeline/Buffers.h"
#include "Hazard/RenderContext/Pipeline/Pipeline.h"
#include "Texture.h"
#include "Camera.h"
#include "WorldRenderer.h"

namespace Hazard::Rendering {

	class DebugRenderer;
	struct RenderPassData
	{
		glm::mat4 ViewProjection;
		glm::mat4 Projection;
		glm::mat4 View;
		glm::vec4 Position;
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

		template<typename FuncT>
		void Submit(FuncT&& fn)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				pFunc->~FuncT();
			};

			auto storageBuffer = m_Queue->Allocate(renderCmd, sizeof(fn));
			new (storageBuffer) FuncT(std::forward<FuncT>(fn));
		}
		template<typename FuncT>
		void SubmitPostPass(FuncT&& fn)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				pFunc->~FuncT();
			};

			auto storageBuffer = m_PostPassQueue->Allocate(renderCmd, sizeof(fn));
			new (storageBuffer) FuncT(std::forward<FuncT>(fn));
		}


		bool OnResize(WindowResizeEvent& e);

		Renderer2D& Get2D() { return *m_Renderer2D; }
		DebugRenderer& GetDebugRenderer() { return *m_DebugRenderer; }
		Ref<RenderPass> GetCurrentRenderPass() { return m_CurrentRenderPass; }
		Ref<RenderCommandBuffer> GetCurrentRenderCommandBuffer() { return m_RenderCommandBuffer; }
	private:
		void ProcessWorldRenderer(WorldRenderer* renderer);

	private:
		Renderer2D* m_Renderer2D;
		DebugRenderer* m_DebugRenderer;
		Ref<Texture2D> m_WhiteTexture;
		Ref<RenderCommandBuffer> m_RenderCommandBuffer;
		Ref<RenderPass> m_CurrentRenderPass;
		Ref<UniformBuffer> m_CameraUBO;
		CommandQueue* m_PostPassQueue;
		CommandQueue* m_Queue;
		RenderPassData m_RenderPassData;
	};
}