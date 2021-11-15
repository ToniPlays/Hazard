#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Pipeline/Buffers.h"
#include "Texture.h"
#include "Mesh/Mesh.h"
#include "Image/Image2D.h"
#include "Pipeline/FrameBuffer.h"

namespace Hazard::Rendering {

	struct RenderPassData {
		glm::mat4 ViewProjection;
		glm::vec4 CameraPosition;
	};

	class Renderer2D;

	class RenderEngine : public Module::Module
	{
	public:
		RenderEngine(RenderEngineCreateInfo* info, RenderAPI api);
		~RenderEngine();

		void Render() override;
		bool OnEvent(Event& e) override;
		void Close() override;
		 
		bool OnResize(WindowResizeEvent& e);
		void SetViewportSize(uint32_t width, uint32_t heigth);
		Ref<Image2D> GetFinalPassImage() { return m_RenderPass->GetSpecs().TargetFrameBuffer->GetImage(); }
		Ref<RenderPass> GetRenderPass() { return m_RenderPass; }

	private:
		Renderer2D* m_Renderer2D;
		glm::mat4 m_Projection;
		glm::mat4 m_CameraTransform;
		Ref<Texture2D> m_WhiteTexture;
		Ref<RenderCommandBuffer> m_RenderCommandBuffer;
		Ref<Mesh> m_TestMesh;
		RenderPassData m_RenderPassData;
		Ref<RenderPass> m_RenderPass;

		double* deltaTime;
	};
}