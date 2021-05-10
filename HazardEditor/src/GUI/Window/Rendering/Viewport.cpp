#pragma once

#include <hzreditor.h>
#include "Viewport.h"
#include "GUI/Library/Layout/Layout.h"

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
		m_Renderer = &Application::GetModule<RenderEngine>(found);
		SetActive(found);

		TextureSpecs specs;
		specs.dataType = TextureDataType::RGBA;

		m_RenderTexture = RenderUtils::Create<RenderTexture>(specs);
		
	}
	void Viewport::OnWindowRender()
	{
		ECS::World& world = ECS::SceneCommand::GetCurrentWorld();
		auto&[found, camera, t] = world.GetWorldCamera();

		if (found) {
			Rendering::RenderCommand::SetRenderTarget(m_RenderTexture);
			ECS::SceneCommand::RenderScene(Rendering::Camera(m_EditorCamera.GetProjection(), glm::inverse(m_EditorCamera.GetView()),
				m_EditorCamera.GetPosition()));
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImVec2 size = ImGui::GetContentRegionAvail();
		if (size.x != m_Width || size.y != m_Height) {
			m_Width = size.x;
			m_Height = size.y;

			m_RenderTexture->Resize(m_Width, m_Height);
			m_EditorCamera.SetViewpotSize(m_Width, m_Height);
		}

		ImGui::Image((void*)m_RenderTexture->GetColorID(),
			size, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PopStyleVar();

		m_Gizmos.OnRender(m_EditorCamera);

		if (m_Gizmos.IsUsing()) return;
		
		IsFocused() ? m_EditorCamera.OnUpdate() : m_EditorCamera.SetMousePosition(Input::GetMousePos());
	}
	bool Viewport::OnEvent(Event& e)
	{
		if(IsHovered())
			m_EditorCamera.OnEvent(e);
		m_Gizmos.OnEvent(e);

		if (!IsHovered()) return false;

		EventDispatcher dispacher(e);
		dispacher.Dispatch<KeyPressedEvent>(BIND_EVENT(Viewport::KeyPressed));

		return false;
	}
	bool Viewport::KeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode()) {
		case Key::Q:
			m_Gizmos.SetType(Gizmo::Translate);
			return true;
		case Key::W:
			m_Gizmos.SetType(Gizmo::Rotate);
			return true;
		case Key::E:
			m_Gizmos.SetType(Gizmo::Scale);
			return true;
		case Key::R:
			m_Gizmos.SetType(Gizmo::Bounds);
			return true;
		case Key::G:
			m_Gizmos.SetGlobal(!m_Gizmos.IsGlobal());
			return true;
		}
		return false;
	}
}