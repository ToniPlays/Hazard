#pragma once

#include "Hazard/Module.h"
#include "HazardRenderer.h"
#include "RendererDrawList.h"

namespace Hazard 
{
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

		HazardRenderer::Window& GetWindow() { return *m_Window; }
		RendererDrawList& GetDrawList() { return m_DrawList; }
	private:
		HazardRenderer::Window* m_Window;
		RendererDrawList m_DrawList;
	};
}
