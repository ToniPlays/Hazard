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

	}
	void Viewport::OnWindowRender()
	{
		ECS::Scene& scene = ECS::SceneCommand::GetCurrentScene();
		auto&[found, camera, t] = scene.GetSceneCamera();

		Rendering::RenderCommand::SetRenderTarget(renderTexture);
		ECS::SceneCommand::RenderScene(Rendering::Camera(editorCamera.GetProjection(), glm::inverse(editorCamera.GetView()), 
			editorCamera.GetPosition(), camera.bgColor.ToGlm()));


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
		case Key::G:
			gizmos.SetGlobal(!gizmos.IsGlobal());
			return true;
		}
		return false;
	}
}