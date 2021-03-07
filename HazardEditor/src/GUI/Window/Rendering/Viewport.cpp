#pragma once

#include <hzreditor.h>
#include "Viewport.h"
#include "GUI/Library/Layout.h"

using namespace Hazard;

namespace WindowElement {

	Viewport::Viewport() : EditorWindow(ICON_FK_GLOBE " Viewport")
	{

	}
	Viewport::~Viewport()
	{
		
	}
	void Viewport::Init()
	{
		bool found = false;
		renderer = &Application::GetModule<RenderEngine>(found);
		SetActive(found);
		renderTexture = RenderUtils::Create<RenderTexture>();

		if (found) renderer->SetRenderTarget(RenderUtils::Create<RenderTexture>());
	}
	void Viewport::OnWindowRender()
	{
		Hazard::ECS::Scene& scene = Application::GetModule<ECS::SceneHandler>().GetCurrentScene();
		renderer->SetRenderTarget(renderTexture);
		renderer->SceneRender(scene, editorCamera.GetViewPprojection());

		RenderTexture* texture = renderer->GetRenderTarget();
		if (texture == nullptr) 
			return;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

		ImVec2 size = ImGui::GetContentRegionAvail();
		if (size.x != width || size.y != height) {
			width = size.x;
			height = size.y;

			renderTexture->Resize(width, height);
			editorCamera.SetViewpotSize(width, height);
		}

		ImGui::Image((void*)renderTexture->GetColorID(),
			size, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PopStyleVar();

		gizmos.OnRender(editorCamera);

		if (gizmos.IsUsing()) return;
		
		IsFocused() ? editorCamera.OnUpdate() : editorCamera.SetMousePosition(Input::GetMousePos());
	}
	bool Viewport::OnEvent(Event& e)
	{
		if(IsHovered())
			editorCamera.OnEvent(e);
		gizmos.OnEvent(e);

		if (!IsHovered()) return false;

		EventDispatcher dispacher(e);
		dispacher.Dispatch<KeyPressedEvent>(BIND_EVENT(Viewport::KeyPressed));

		return false;
	}
	bool Viewport::KeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode()) {
		case Key::D1:
			gizmos.SetType(Gizmo::Translate);
			return true;
		case Key::D2:
			gizmos.SetType(Gizmo::Rotate);
			return true;
		case Key::D3:
			gizmos.SetType(Gizmo::Scale);
			return true;
		case Key::D4:
			gizmos.SetType(Gizmo::Bounds);
			return true;
		}
		return false;
	}
}