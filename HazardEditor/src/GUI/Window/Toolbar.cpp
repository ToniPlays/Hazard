#pragma once
#include <hzreditor.h>
#include "Toolbar.h"

#include "GUI/Library/Layout.h"
#include "GUI/Library/Input.h"

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
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
		ImGui::SetWindowSize(ImVec2{ ImGui::GetWindowWidth(), 50.0f });
		ImGui::Begin("Toolbar", false, flags);

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 16 * 3);
		if (Input::Button(ICON_FK_PLAY, { 32, 32 })) {
			
		}
		Layout::SameLine(0, 5);
		if (Input::Button(ICON_FK_PAUSE, { 32, 32 })) {
			
		}
		Layout::SameLine(0, 5);
		if (Input::Button(ICON_FK_FORWARD, { 32, 32 })) {
			Application::GetModule < Scripting::ScriptCommand>().DoStep();
		}

		ImGui::End();
	}
}
