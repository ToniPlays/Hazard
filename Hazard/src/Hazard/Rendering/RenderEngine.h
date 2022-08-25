#pragma once

#include "Hazard/Module.h"
#include "Hazard/Core/Core.h"
#include "HazardRenderer.h"
#include "RendererDrawList.h"
#include "Renderers/QuadRenderer.h"
#include "Renderers/LineRenderer.h"

namespace Hazard 
{
	struct CameraData 
	{
		glm::mat4 ViewProjection;
		glm::mat4 Projection;
		glm::mat4 View;
		glm::vec4 Position;
	};

	struct DirectionalLight {
		//W component not used
		glm::vec4 Direction;
		//Alpha is intensity
		glm::vec4 Color;
	};

	struct LightingData 
	{
		int DirectionLightCount;
		int PointLightCount;
		int Padding;
		int Padding2;
		DirectionalLight Lights[16];
	};


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
		void ShadowPass();
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

		Ref<Texture2D> GetWhiteTexture() { return m_WhiteTexture; };

	private:
		HazardRenderer::Window* m_Window;
		std::vector<RendererDrawList> m_DrawList;

		Ref<HazardRenderer::UniformBuffer> m_CameraUniformBuffer;
		Ref<HazardRenderer::UniformBuffer> m_LightUniformBuffer;
		Ref<HazardRenderer::UniformBuffer> m_ModelUniformBuffer;

		Ref<HazardRenderer::FrameBuffer> m_DeferredFrameBuffer;
		Ref<HazardRenderer::RenderPass> m_DeferredRenderPass;

		Ref<HazardRenderer::Pipeline> m_CompositePipeline;

		Ref<Texture2D> m_WhiteTexture;

		QuadRenderer m_QuadRenderer;
		LineRenderer m_LineRenderer;

		uint32_t m_CurrentDrawContext = 0;
		
	};
}
