#pragma once

#include "Hazard/Module.h"
#include "Hazard/Core/Core.h"
#include "RenderResources.h"
#include "RendererDrawList.h"
#include "Renderers/QuadRenderer.h"
#include "Renderers/LineRenderer.h"

namespace Hazard 
{

	class RenderEngine : public Module 
	{
	public:
		RenderEngine() = default;
		RenderEngine(HazardRenderer::HazardRendererCreateInfo* createInfo);
		~RenderEngine() = default;

		/// <summary>
		/// Get all the geometry for rendering
		/// </summary>
		/// <param name="renderer"></param>
		void PreRender(Ref<WorldRenderer> renderer);
		void ShadowPass(Ref<RenderCommandBuffer> commandBuffer);
		void GeometryPass(const Ref<HazardRenderer::RenderCommandBuffer>& cmdBuffer);
		void CompositePass(Ref<HazardRenderer::RenderCommandBuffer> commandBuffer);

		void PrepareLights();

        void Update() override;
		void Render() override;
		void PostRender() override;

		QuadRenderer& GetQuadRenderer() { return m_QuadRenderer; }
		LineRenderer& GetLineRenderer() { return m_LineRenderer; }
		Ref<HazardRenderer::FrameBuffer>& GetDeferredFramebuffer() { return m_DeferredFrameBuffer; }

		HazardRenderer::Window& GetWindow() { return *m_Window; }
		RendererDrawList& GetDrawList() { return m_DrawList[m_CurrentDrawContext]; }
		std::vector<RendererDrawList>& GetDrawLists() { return m_DrawList; }

		Ref<Texture2DAsset> GetWhiteTexture() { return m_WhiteTexture; };

	private:
		HazardRenderer::Window* m_Window;
		std::vector<RendererDrawList> m_DrawList;

		RenderResources* m_Resources = nullptr;

		Ref<HazardRenderer::FrameBuffer> m_DeferredFrameBuffer;
		Ref<HazardRenderer::RenderPass> m_DeferredRenderPass;

		Ref<Texture2DAsset> m_WhiteTexture;

		QuadRenderer m_QuadRenderer;
		LineRenderer m_LineRenderer;

		uint32_t m_CurrentDrawContext = 0;
		
	};
}
