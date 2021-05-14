#pragma once
#include <hzreditor.h>
#include "Toolbar.h"

#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Style.h"
#include "GUI/EditorView.h"
#include "Core/SceneRuntimeHandler.h"

#include "Gui/Window/Rendering/Viewport.h"
#include "Gui/Window/Rendering/GameViewport.h"
#include "Gui/Window/Rendering/WorldEnvironmentData.h"

using namespace WindowLayout;

namespace WindowElement {

	Toolbar::Toolbar()
	{

	}
	Toolbar::~Toolbar()
	{

	}
	void Toolbar::Init()
	{
		Runtime::SceneRuntimeHandler::Init();
	}
	void Toolbar::OnRender()
	{
		using namespace Appereance;

		Color onColor = Color::FromHex("#404040");
		Color offColor = Style::GetStyleColor(ColorType::Secondary);

		TransformationGizmo& gizmo = EditorView::GetInstance().GetRenderable<Viewport>()->GetGizmo();
		static bool b = false;

		ImGui::Begin("##Toolbar");

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
		if (Input::ButtonColorChange(ICON_FK_CROSSHAIRS, offColor, onColor, Style::GetStyleColor(ColorType::Warning), gizmo.ForcedSnapping(), { 28, 28 })) {
			gizmo.SetForcedSnapping(!gizmo.ForcedSnapping());
		}

		Layout::SameLine(0, 5);

		bool sceneRunning = Runtime::SceneRuntimeHandler::IsSceneRunning();
		bool scenePaused = Runtime::SceneRuntimeHandler::IsScenePaused();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 16 * 3);

		if (Input::ButtonColorChange(ICON_FK_PLAY, offColor, onColor, Style::GetStyleColor(ColorType::Text), sceneRunning, { 28, 28 })) {
			Runtime::SceneRuntimeHandler::SetSceneRunning(!sceneRunning);

			if(!sceneRunning)
				EditorView::SetWindowFocus<GameViewport>();
			else 
				EditorView::SetWindowFocus<Viewport>();
		}
		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_PAUSE, offColor, onColor, Style::GetStyleColor(ColorType::Text), scenePaused, { 28, 28 })) {
			Runtime::SceneRuntimeHandler::SetScenePaused(!scenePaused);
		}
		Layout::SameLine(0, 5);

		if(Input::Button(ICON_FK_FORWARD, { 28, 28 })) {
			if (sceneRunning && scenePaused)
				Application::GetModule<ScriptEngineManager>()->Update();
		}
		Layout::SameLine(0, 5);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 33 * 3);
		if (Input::ButtonColorChange(ICON_FK_LIGHTBULB_O, offColor, onColor, Style::GetStyleColor(ColorType::Warning), Rendering::RenderContextCommand::IsVsync(), { 28, 28 })) {
			EditorView::GetInstance().SetLayerActive<WorldEnvironmentData>(true);
		}

		Layout::SameLine(0, 5);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 33 * 2);
		if(Input::ButtonColorChange(ICON_FK_SLIDERS, offColor, onColor, Style::GetStyleColor(ColorType::Debug), Rendering::RenderContextCommand::IsVsync(), { 28, 28 })) {
			Rendering::RenderContextCommand::SetVsync(!Rendering::RenderContextCommand::IsVsync());
		}
		Layout::Tooltip("VSync");

		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_COGS, offColor, onColor, Style::GetStyleColor(ColorType::Critical), false, { 28, 28 })) {
			ImGui::SaveIniSettingsToDisk("imgui.ini");
		}
		ImGui::End();
	}
	bool Toolbar::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Toolbar::OnKey));
	}
	bool Toolbar::OnKey(KeyPressedEvent& e)
	{
		bool isCtrl = Hazard::Input::IsKeyDown(Key::LeftControl);
		bool isShift = Hazard::Input::IsKeyDown(Key::LeftShift);

		if (e.GetKeyCode() == Key::R) {
			Application::GetModule<ScriptEngineManager>()->ReloadAll();
			return true;
		}
		return false;
	}
}
