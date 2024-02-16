#pragma once

#include "Hazard/Module.h"
#include "Hazard/Core/Core.h"
#include "RenderResources.h"
#include "RendererDrawList.h"
#include "Renderers/QuadRenderer.h"
#include "Renderers/LineRenderer.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Renderers/CircleRenderer.h"
#include "Renderers/GeometryRenderer.h"
#include "Renderers/EnvironmentRenderer.h"

namespace Hazard 
{
	class RenderEngine : public Module 
	{
	public:
		RenderEngine() = delete;
		RenderEngine(RendererCreateInfo* createInfo);
		~RenderEngine() = default;

		void Init() override;
        void Update() override;
		void Render() override;

		void ClearDrawLists();
		void CollectGeometry();

		QuadRenderer& GetQuadRenderer() { return m_QuadRenderer; }
		LineRenderer& GetLineRenderer() { return m_LineRenderer; }
		CircleRenderer& GetCircleRenderer() { return m_CircleRenderer; }

		RendererDrawList& GetDrawList() { return m_DrawList[m_CurrentDrawContext]; }
		std::vector<RendererDrawList>& GetDrawLists() { return m_DrawList; }
		static RenderResources& GetResources() { return *s_Resources; }

	private:
		std::vector<RendererDrawList> m_DrawList;
        
		QuadRenderer m_QuadRenderer;
		LineRenderer m_LineRenderer;
		CircleRenderer m_CircleRenderer;
		GeometryRenderer m_GeometryRenderer;
		EnvironmentRenderer m_EnvironmentRenderer;

		RenderContextManager* m_RenderContextManager;
		uint32_t m_CurrentDrawContext = 0;

		inline static RenderResources* s_Resources = nullptr;
	};
}
