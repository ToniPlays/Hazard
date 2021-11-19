#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "RenderCommandBuffer.h"
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
		RenderEngine(RenderEngineCreateInfo* info, RenderAPI api);
		~RenderEngine();

		void Render() override;
		bool OnEvent(Event& e) override;
		void Close() override;
		 
		bool OnResize(WindowResizeEvent& e);
		void SetCamera(Camera* camera) { m_RenderingCamera = camera; }

		Ref<Image2D> GetFinalPassImage() { return m_RenderPass->GetSpecs().TargetFrameBuffer->GetImage(); }
		Ref<RenderPass> GetRenderPass() { return m_RenderPass; }
		RenderStats GetStats() { return m_RenderCommandBuffer->GetStats(); }
		Renderer2D& Get2D() { return *m_Renderer2D; }

	private:
		Renderer2D* m_Renderer2D;
		Ref<Texture2D> m_WhiteTexture;
		Ref<RenderCommandBuffer> m_RenderCommandBuffer;
		RenderPassData m_RenderPassData;
		Ref<RenderPass> m_RenderPass;
		Ref<FrameBuffer> m_FrameBuffer;
		Camera* m_RenderingCamera = nullptr;

	};
}