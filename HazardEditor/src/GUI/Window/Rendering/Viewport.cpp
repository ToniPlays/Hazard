#pragma once

#include <hzreditor.h>
#include "Viewport.h"
#include "GUI/Library/Layout.h"


namespace WindowElement {

	Viewport::Viewport() : EditorWindow("Viewport")
	{

	}
	Viewport::~Viewport()
	{
		
	}
	void Viewport::Init()
	{
		bool found = false;
		renderer = &Hazard::Application::GetModule<RenderEngine>(found);
		SetActive(found);

		if (found) renderer->SetRenderTarget(RenderUtils::Create<RenderTexture>());
	}
	void Viewport::OnWindowRender()
	{
		Hazard::ECS::Scene& scene = Application::GetModule<ECS::SceneHandler>().GetCurrentScene();
		renderer->SceneRender(scene);



		RenderTexture* texture = renderer->GetRenderTarget();
		if (texture == nullptr) 
			return;

		ImVec2 size = ImGui::GetContentRegionAvail();
		if (size.x != width || size.y != height) {
			width = size.x;
			height = size.y;

			renderer->GetRenderTarget()->Resize(width, height);
			scene.GetSceneCamera().RecalculateProjection(width, height);

		}

		ImGui::Image((void*)renderer->GetRenderTarget()->GetColorID(),
			size, ImVec2(0, 1), ImVec2(1, 0));

	}
}