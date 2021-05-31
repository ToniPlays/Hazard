#pragma once

#include <hzreditor.h>
#include "Viewport.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Style.h"
#include "GUI/Window/DragDropUtils.h"
#include "Core/EditorEvent.h"
#include "GUI/EditorView.h"

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
		m_Renderer = Application::GetModule<RenderEngine>(found);
		SetActive(found);

		Rendering::FrameBufferCreateInfo createInfo;
		createInfo.attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::Depth };

		m_RenderTexture = RenderUtils::Create<FrameBuffer>(createInfo);
		m_RenderTexture->Resize(1920, 1080);

	}
	void Viewport::OnWindowRender()
	{
		ECS::World& world = ECS::WorldCommand::GetCurrentWorld();

		bool is2D = m_EditorCamera.Is2DEnabled();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

		ImVec2 corner = ImGui::GetCursorPos();
		ImVec2 size = ImGui::GetContentRegionAvail();

		bool changed = false;

		if (size.x != m_Width || size.y != m_Height) {
			m_Width = size.x;
			m_Height = size.y;

			changed = true;
			m_RenderTexture->Resize(m_Width, m_Height);
			m_EditorCamera.SetViewpotSize(m_Width, m_Height);
		}


		Rendering::RenderCommand::SetFrameBuffer(m_RenderTexture.Raw());
		ECS::WorldCommand::RenderScene(Rendering::Camera(m_EditorCamera.GetProjection(), glm::inverse(m_EditorCamera.GetView()),
			m_EditorCamera.GetPosition()));

		ImGui::Image((void*)m_RenderTexture->GetColorID(0),
			size, ImVec2(0, 1), ImVec2(1, 0));


		DragDropUtils::DragTarget("World", [&](const ImGuiPayload* payload) {
			const char* file = (const char*)payload->Data;
			Events::SelectionContextChange e({ });
			EditorView::GetInstance().OnEvent(e);

			Application::GetModule<ECS::WorldHandler>()->LoadWorld(file);
			});

		m_Gizmos.OnRender(m_EditorCamera, size);

		ImGui::PopStyleVar();

		ImGui::SetCursorPos({ corner.x + 10, corner.y + 5 });
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, FLT_MAX);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 6, 5 });

		if (Input::Button(ICON_FK_COG, { 25, 25 })) {

		}

		ImGui::PopStyleVar();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 8, 2 });
		ImGui::SameLine(0, 15);
		std::string text = m_EditorCamera.Is2DEnabled() ? "2D Projection" : "3D Projection";

		if (Input::Button(text.c_str(), { 0, 25 })) {
			m_EditorCamera.SetIs2D(!m_EditorCamera.Is2DEnabled());
		}

		ImGui::SameLine(0, 15);
		if (Input::Button(ICON_FK_EYE " Show", { 0, 25 })) {

		}

		ImGui::PopStyleVar(2);
		ImGui::SameLine();
		ImGui::SetCursorPosX(size.x - 100);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, Style::ColorAsImVec4(Color::FromHex("#222222")));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 25);

		Color onColor = Style::GetStyleColor(ColorType::Text);
		Color offColor = Style::GetStyleColor(ColorType::Secondary);

		ImGui::BeginChild("##gizmoTools", { 92, 25 });
		ImGui::SameLine(0, 10);
		if (Input::ButtonColorChange(ICON_FK_ARROWS, offColor, onColor, Style::GetStyleColor(Debug), m_Gizmos.GetType() != Gizmo::Translate, { 0, 25 })) {
			m_Gizmos.SetType(Gizmo::Translate);
		}
		ImGui::SameLine(0, 0);
		if (Input::ButtonColorChange(ICON_FK_REPEAT, offColor, onColor, Style::GetStyleColor(Critical), m_Gizmos.GetType() != Gizmo::Rotate, { 0, 25 })) {
			m_Gizmos.SetType(Gizmo::Rotate);
		}
		ImGui::SameLine(0, 0);
		if (Input::ButtonColorChange(ICON_FK_EXPAND, offColor, onColor, Style::GetStyleColor(Warning), m_Gizmos.GetType() != Gizmo::Scale, { 0, 25 })) {
			m_Gizmos.SetType(Gizmo::Scale);
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		if (m_Gizmos.IsUsing()) return;

		IsFocused() ? m_EditorCamera.OnUpdate() : m_EditorCamera.SetMousePosition(Hazard::Input::GetMousePos());
	}
	bool Viewport::OnEvent(Event& e)
	{
		if (IsHovered())
			m_EditorCamera.OnEvent(e);
		m_Gizmos.OnEvent(e);

		if (!IsHovered()) return false;

		EventDispatcher dispacher(e);
		return dispacher.Dispatch<KeyPressedEvent>(BIND_EVENT(Viewport::KeyPressed));
	}
	bool Viewport::KeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode()) {
		case Key::W:
			m_Gizmos.SetType(Gizmo::Translate);
			return true;
		case Key::E:
			m_Gizmos.SetType(Gizmo::Rotate);
			return true;
		case Key::R:
			m_Gizmos.SetType(Gizmo::Scale);
			return true;
		case Key::T:
			m_Gizmos.SetType(Gizmo::Bounds);
			return true;
		case Key::G:
			m_Gizmos.SetGlobal(!m_Gizmos.IsGlobal());
			return true;
		}
		return false;
	}
}