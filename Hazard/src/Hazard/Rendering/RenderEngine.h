#pragma once

#include "Hazard/Module.h"
#include "Hazard/Core/Core.h"
#include "RenderResources.h"
#include "RendererDrawList.h"
#include "Renderers/QuadRenderer.h"
#include "Renderers/LineRenderer.h"
#include "Hazard/RenderContext/RenderContextManager.h"

#include "Hazard/Core/ApplicationCreateInfo.h"

namespace Hazard 
{

	class RenderEngine : public Module 
	{
	public:
		RenderEngine() = default;
		RenderEngine(RendererCreateInfo* createInfo);
		~RenderEngine() = default;

        void Update() override;
		void Render() override;
		/// <summary>
		/// Get all the geometry for rendering
		/// </summary>
		/// <param name="renderer"></param>

		void ClearDrawLists();

		void PreRender();
		void ShadowPass(Ref<RenderCommandBuffer> commandBuffer);
		void PreDepthPass(Ref<RenderCommandBuffer> commandBuffer);
		void GeometryPass(Ref<RenderCommandBuffer> commandBuffer);
		void CompositePass(Ref<RenderCommandBuffer> commandBuffer);

		//Not yet implemented
		void LightCullingPass(Ref<RenderCommandBuffer> commandBuffer) {};

		QuadRenderer& GetQuadRenderer() { return m_QuadRenderer; }
		LineRenderer& GetLineRenderer() { return m_LineRenderer; }

		RendererDrawList& GetDrawList() { return m_DrawList[m_CurrentDrawContext]; }
		std::vector<RendererDrawList>& GetDrawLists() { return m_DrawList; }

		Ref<Texture2DAsset> GetWhiteTexture() { return m_WhiteTexture; };

	private:
		std::vector<RendererDrawList> m_DrawList;
		RenderContextManager* m_RenderContextManager;

		RenderResources* m_Resources = nullptr;
		Ref<Image2DAsset> m_WhiteTexture;

		QuadRenderer m_QuadRenderer;
		LineRenderer m_LineRenderer;

		uint32_t m_CurrentDrawContext = 0;
		
	};
}
