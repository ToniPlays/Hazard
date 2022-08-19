#pragma once

#include "Hazard/Module.h"
#include "HazardRenderer.h"
#include "RendererDrawList.h"
#include "QuadRenderer.h"
#include "LineRenderer.h"

namespace Hazard 
{
	struct CameraData 
	{
		glm::mat4 ViewProjection;
		glm::mat4 Projection;
		glm::mat4 View;
		glm::vec4 Position;
	};

	class RenderEngine : public Module 
	{
	public:
		RenderEngine() = default;
		RenderEngine(HazardRenderer::HazardRendererCreateInfo* createInfo);
		~RenderEngine() = default;

		void CullingPass();
		void GeometryPass();
		void ShadowPass();
		void CompositePass();

        void Update() override;
		void Render() override;
		void PostRender() override;

		QuadRenderer& GetQuadRenderer() { return m_QuadRenderer; }
		LineRenderer& GetLineRenderer() { return m_LineRenderer; }

		HazardRenderer::Window& GetWindow() { return *m_Window; }
		RendererDrawList& GetDrawList() { return m_DrawList; }

		Ref<Texture2D> GetWhiteTexture() { return m_WhiteTexture; };

	private:
		HazardRenderer::Window* m_Window;
		RendererDrawList m_DrawList;
		Ref<HazardRenderer::UniformBuffer> m_CameraUniformBuffer;
		Ref<HazardRenderer::UniformBuffer> m_ModelUniformBuffer;

		Ref<Texture2D> m_WhiteTexture;

		QuadRenderer m_QuadRenderer;
		LineRenderer m_LineRenderer;
	};
}
