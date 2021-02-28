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
		renderer->SceneRender(Application::GetModule<ECS::SceneHandler>().GetCurrentScene());

		RenderTexture* texture = renderer->GetRenderTarget();
		if (texture == nullptr) 
			return;

		ImVec2 size = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)renderer->GetRenderTarget()->GetColorID(),
			size, ImVec2(0, 1), ImVec2(1, 0));
	}
}