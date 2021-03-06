#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"
#include "RenderUtils.h"
#include "2D/QuadData.h"


namespace Hazard::Rendering {

	RenderEngine::RenderEngine() : Module("RenderEngine")
	{
		SetActive(true);
	}
	RenderEngine::~RenderEngine()
	{

	}
	void RenderEngine::PreInit()
	{
		
	}
	void RenderEngine::Init()
	{
		bool found = false;
		context = &Application::GetModule<RenderContext>(found);
		HZR_CORE_ASSERT(found, "RenderEngine cannot start without RenderContext");
		SetActive(found);
		if (!found) return;

		RenderUtils::Init();
		occlusionQuery = RenderUtils::Create<OcclusionQuery>();

		renderer2D = new Renderer2D(context);
		renderer2D->Init(35000);

		occlusionQuery->BeginQuery();
	}
	void RenderEngine::Close()
	{
		occlusionQuery->EndQuery();
		occlusionQuery->Flush();
		renderer2D->Close();
	}
	void RenderEngine::SceneRender(ECS::Scene& scene, glm::mat4 viewProjection)
	{

		stats.drawCalls = 0;
		stats.quads = 0;
		stats.indices = 0;
		stats.vertices = 0;

		if (renderTarget == nullptr) 
			return;

		renderTarget->Bind();
		context->GetWindow().GetContext()->ClearFrame("#222222");
		renderer2D->BeginScene(viewProjection);
		renderer2D->BeginBatch();
		scene.Render();
		//Process scene

		renderer2D->Flush();
		renderTarget->Unbind();

		renderer2D->CollectStats(stats);
	}
}