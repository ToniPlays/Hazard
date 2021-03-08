#pragma once
#include <hzreditor.h>
#include "Toolbar.h"

#include "GUI/Library/Layout.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Style.h"

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

		using namespace Appereance;

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
		ImGui::SetWindowSize(ImVec2{ ImGui::GetWindowWidth(), 50.0f });
		ImGui::Begin("Toolbar", false, flags);

		static bool b = false;

		Color onColor = Color::FromHex("#404040");
		Color offColor = Style::GetStyleColor(ColorType::Secondary);

		ImGui::SameLine(5);
		Input::ToggleButtonColorChange(ICON_FK_ARROWS, offColor, onColor, Style::GetStyleColor(ColorType::Debug), b, { 32, 32 });
		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_REPEAT, offColor, onColor, Style::GetStyleColor(ColorType::Critical), b, { 32, 32 });
		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_EXPAND, offColor, onColor, Style::GetStyleColor(ColorType::Warning), b, { 32, 32 });
		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_CODEPEN, offColor, onColor, Style::GetStyleColor(ColorType::Info), b, { 32, 32 });
		Layout::SameLine(0, 5);


		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 16 * 3);
		Input::ToggleButtonColorChange(ICON_FK_PLAY, offColor, onColor, Style::GetStyleColor(ColorType::Text), b, { 32, 32 });
		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_PAUSE, offColor, onColor, Style::GetStyleColor(ColorType::Text), b, { 32, 32 });
		Layout::SameLine(0, 5);

		if(Input::Button(ICON_FK_FORWARD, { 32, 32 })) {
			Application::GetModule < Scripting::ScriptCommand>().DoStep();
		}

		ImGui::End();
	}
}
