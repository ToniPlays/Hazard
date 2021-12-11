#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "RenderCommandBuffer.h"
#include "RenderCommandQueue.h"
#include "Pipeline/Buffers.h"
#include "Texture.h"
#include "Mesh/Mesh.h"
#include "Image/Image2D.h"
#include "Pipeline/FrameBuffer.h"
#include "Camera.h"

namespace Hazard::Rendering {

	struct RenderPassData {
		glm::mat4 ViewProjection;
		glm::vec4 CameraPosition;
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

				pFunc->~Func();
			};

			auto storageBuffer = m_Queue->Allocate(renderCmd, sizeof(fn));
			new (storageBuffer) Func(std::forward<Func>(fn));
		}
		 
		bool OnResize(WindowResizeEvent& e);
		RenderStats GetStats() { return m_RenderCommandBuffer->GetStats(); }

		Renderer2D& Get2D() { return *m_Renderer2D; }

	private:
		Renderer2D* m_Renderer2D;
		Ref<Texture2D> m_WhiteTexture;
		Ref<RenderCommandBuffer> m_RenderCommandBuffer;
		RenderCommandQueue* m_Queue;
		RenderPassData m_RenderPassData;
	};
}