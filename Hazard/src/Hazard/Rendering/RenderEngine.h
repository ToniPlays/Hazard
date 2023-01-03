#pragma once

#include "Hazard/Module.h"
#include "Hazard/Core/Core.h"
#include "RenderResources.h"
#include "RendererDrawList.h"
#include "Renderers/QuadRenderer.h"
#include "Renderers/LineRenderer.h"
#include "Hazard/RenderContext/RenderContextManager.h"

#include "Rasterized/RasterizedRenderer.h"
#include "Raytraced/RaytracedRenderer.h"

#include "Hazard/Core/ApplicationCreateInfo.h"

namespace Hazard 
{
	enum RendererFlags : uint32_t
	{
		RendererFlags_None = 0,
		RendererFlags_Overdraw = BIT(0),
	};

	struct RendererSettings
	{
		bool Raytraced = false;
	};

	class RenderEngine : public Module 
	{
	public:
		RenderEngine() = delete;
		RenderEngine(RendererCreateInfo* createInfo);
		~RenderEngine() = default;

        void Update() override;
		void Render() override;

		void ClearDrawLists();

		/// <summary>
		/// Get all the geometry for rendering
		/// </summary>
		/// <param name="renderer"></param>
		void CollectGeometry();

		//Not yet implemented, NOT TODO

		uint32_t GetFlags() { return m_Flags; }
		void SetFlags(uint32_t flags) { m_Flags = flags; }

		QuadRenderer& GetQuadRenderer() { return m_QuadRenderer; }
		LineRenderer& GetLineRenderer() { return m_LineRenderer; }

		RendererDrawList& GetDrawList() { return m_DrawList[m_CurrentDrawContext]; }
		std::vector<RendererDrawList>& GetDrawLists() { return m_DrawList; }
		Ref<HazardRenderer::RenderPass> GetRenderPass() { return m_RenderPass; }
		
		static RenderResources& GetResources() { return *s_Resources; }
		RendererSettings& GetSettings() { return m_Settings; }

	private:
		std::vector<RendererDrawList> m_DrawList;
        
		RenderContextManager* m_RenderContextManager;

		inline static RenderResources* s_Resources = nullptr;
		RasterizedRenderer* m_RasterizedRenderer;
		RaytracedRenderer* m_RaytracedRenderer;

		QuadRenderer m_QuadRenderer;
		LineRenderer m_LineRenderer;

		Ref<HazardRenderer::FrameBuffer> m_FrameBuffer;
		Ref<HazardRenderer::RenderPass> m_RenderPass;

		uint32_t m_CurrentDrawContext = 0;
		uint32_t m_Flags = RendererFlags_None;
		RendererSettings m_Settings;
	};
}
