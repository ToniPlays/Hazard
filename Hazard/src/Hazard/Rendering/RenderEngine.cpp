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
		Flush();
	}
	void RenderEngine::PreInit()
	{
		
	}
	void RenderEngine::Init()
	{
		bool found = false;
		context = &Core::HazardLoop::GetModule<RenderContext>(found);
		HZR_CORE_ASSERT(found, "RenderEngine cannot start without RenderContext");
		SetActive(found);
		renderer2D = new Renderer2D(context);
		renderer2D->Init(35000);

	}
	void RenderEngine::Flush()
	{
		delete context; 
		renderer2D->Close();
	}
	void RenderEngine::SceneRender(ECS::Scene& scene)
	{
		stats.drawCalls = 0;
		stats.quads = 0;
		stats.indices = 0;
		stats.vertices = 0;

		if (renderTarget == nullptr) 
			return;

		renderTarget->Bind();
		context->GetWindow().GetContext()->ClearFrame("#222222");
		renderer2D->BeginScene(scene.GetSceneCamera().projection);
		renderer2D->BeginBatch();

		for (int x = 0; x < 100; x++) {
			for (int y = 0; y < 100; y++) {
				Submit(Quad({ float(x) * 1.05f - 5.0f, float(y) * 1.05 - 5.0f, 0 }, { float(x) * 0.001f, float(y) * 0.001f, 0.2f, 1.0f }));
			}
		}
	
		renderer2D->Flush();
		renderTarget->Unbind();
		renderer2D->CollectStats(stats);
	}
}