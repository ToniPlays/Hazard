#pragma once

#include <hzreditor.h>
#include "Viewport.h"
#include "GUI/Library/Layout.h"

using namespace Hazard;

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
		renderer = &Application::GetModule<RenderEngine>(found);
		SetActive(found);

		if (found) renderer->SetRenderTarget(RenderUtils::Create<RenderTexture>());
	}
	void Viewport::OnWindowRender()
	{
		Hazard::ECS::Scene& scene = Application::GetModule<ECS::SceneHandler>().GetCurrentScene();
		renderer->SceneRender(scene, editorCamera.GetViewPprojection());

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
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

		ImGui::Image((void*)renderer->GetRenderTarget()->GetColorID(),
			size, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::PopStyleVar();
		gizmos.OnRender(editorCamera);

		if (gizmos.IsUsing()) return;
		if (!IsFocused()) return;
		editorCamera.OnUpdate();

	}
	bool Viewport::OnEvent(Event& e)
	{

		if (!IsFocused()) return false;
		EventDispatcher dispacher(e);
		dispacher.Dispatch<KeyPressedEvent>(BIND_EVENT(Viewport::KeyPressed));
		editorCamera.OnEvent(e);
		return gizmos.OnEvent(e);
	}
	bool Viewport::KeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode()) {
		case Key::Q:
			gizmos.SetType(Gizmo::Translate);
			return true;
		case Key::W:
			gizmos.SetType(Gizmo::Rotate);
			return true;
		case Key::E:
			gizmos.SetType(Gizmo::Scale);
			return true;
		case Key::R:
			gizmos.SetType(Gizmo::Bounds);
			return true;
		}
		return false;
	}
}