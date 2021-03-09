#pragma once
#include <hzreditor.h>
#include "Toolbar.h"

#include "GUI/Library/Layout.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Style.h"

#include "GUI/EditorView.h"

#include "Gui/Window/Rendering/Viewport.h"

using namespace WindowLayout;

namespace WindowElement {

	Toobar::Toobar()
	{

	}
	Toobar::~Toobar()
	{
	}

	void Toobar::Init()
	{
	}
	void Toobar::OnRender()
	{

		TransformationGizmo& gizmo = EditorView::GetInstance().GetRenderable<Viewport>().GetGizmo();
		using namespace Appereance;

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
		ImGui::SetWindowSize(ImVec2{ ImGui::GetWindowWidth(), 50.0f });
		ImGui::Begin("Toolbar", false, flags);

		static bool b = false;

		Color onColor = Color::FromHex("#404040");
		Color offColor = Style::GetStyleColor(ColorType::Secondary);

		ImGui::SameLine(5);
		if(Input::ButtonColorChange(ICON_FK_ARROWS, offColor, onColor, Style::GetStyleColor(ColorType::Debug), gizmo.GetType() == Gizmo::Translate, { 28, 28 })) {
			gizmo.SetType(Gizmo::Translate);
		}
		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_REPEAT, offColor, onColor, Style::GetStyleColor(ColorType::Critical), gizmo.GetType() == Gizmo::Rotate, { 28, 28 })) {
			gizmo.SetType(Gizmo::Rotate);
		}
		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_EXPAND, offColor, onColor, Style::GetStyleColor(ColorType::Warning), gizmo.GetType() == Gizmo::Scale, { 28, 28 })) {
			gizmo.SetType(Gizmo::Scale);
		}
		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_CODEPEN, offColor, onColor, Style::GetStyleColor(ColorType::Info), gizmo.GetType() == Gizmo::Bounds, { 28, 28 })) {
			gizmo.SetType(Gizmo::Bounds);
		}
		Layout::SameLine(0, 25);
		if (Input::ButtonColorChange(ICON_FK_GLOBE, offColor, onColor, Style::GetStyleColor(ColorType::Error), gizmo.IsGlobal(), { 28, 28 })) {
			gizmo.SetGlobal(true);
		}
		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_BUILDING, offColor, onColor, Style::GetStyleColor(ColorType::Debug), !gizmo.IsGlobal(), { 28, 28 })) {
			gizmo.SetGlobal(false);
		}
		Layout::SameLine(0, 5);

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 16 * 3);
		Input::ToggleButtonColorChange(ICON_FK_PLAY, offColor, onColor, Style::GetStyleColor(ColorType::Text), b, { 28, 28 });
		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_PAUSE, offColor, onColor, Style::GetStyleColor(ColorType::Text), b, { 28, 28 });
		Layout::SameLine(0, 5);

		if(Input::Button(ICON_FK_FORWARD, { 28, 28 })) {
			Application::GetModule < Scripting::ScriptCommand>().DoStep();
		}

		Layout::SameLine(0, 5);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 33);
		if(Input::ButtonColorChange(ICON_FK_SLIDERS, offColor, onColor, Style::GetStyleColor(ColorType::Text), Rendering::RenderContextCommand::IsVsync(), { 28, 28 })) {
			Rendering::RenderContextCommand::SetVsync(!Rendering::RenderContextCommand::IsVsync());
		}
		Layout::SameLine(0, 5);

		ImGui::End();
	}
}
