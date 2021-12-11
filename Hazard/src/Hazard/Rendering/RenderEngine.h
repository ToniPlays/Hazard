#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "RenderCommandBuffer.h"
#include "RenderCommandQueue.h"
#include "Pipeline/FrameBuffer.h"
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
		 
		bool OnResize(WindowResizeEvent& e);
		RenderStats GetStats() { return m_RenderCommandBuffer->GetStats(); }

		Renderer2D& Get2D() { return *m_Renderer2D; }
		DebugRenderer& GetDebugRenderer() { return *m_DebugRenderer; }

	private:
		Renderer2D* m_Renderer2D;
		DebugRenderer* m_DebugRenderer;
		Ref<Texture2D> m_WhiteTexture;
		Ref<RenderCommandBuffer> m_RenderCommandBuffer;
		RenderCommandQueue* m_Queue;
		RenderPassData m_RenderPassData;
	};
}