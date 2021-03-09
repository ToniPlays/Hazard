#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"
#include "Hazard/RenderContext/RenderUtils.h"
#include "2D/QuadData.h"
#include "RenderCommand.h"

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
		renderer2D = new Renderer2D(&RenderContextCommand::GetContext());
		renderer2D->Init(35000);

		RenderCommand::Init();
	}
	void RenderEngine::Close()
	{
		renderer2D->Close();
	}
	void RenderEngine::SceneRender(ECS::Scene& scene)
	{
		auto [found, cam] = scene.GetSceneCamera();
		if (!found) return;
		SceneRender(scene, cam);
	}
	void RenderEngine::SceneRender(ECS::Scene& scene, ECS::Camera camera)
	{
		if (renderTarget == nullptr) 
			return;

		renderTarget->Bind();
		RenderContextCommand::ClearFrame(camera.component.bgColor);
		renderer2D->BeginScene(camera.viewProjection);
		renderer2D->BeginBatch();
		scene.Render();

		renderer2D->Flush();
		renderTarget->Unbind();
	}
}