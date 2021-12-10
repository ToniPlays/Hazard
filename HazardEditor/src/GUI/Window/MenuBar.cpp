#pragma once


#include "MenuBar.h"

#include "Library/Layout/Layout.h"
#include "Library/Style.h"
#include "Library/Input.h"
#include "Project/ProjectManager.h"

#include "GUI/EditorView.h"
#include "GUI/Window/AllWindows.h"
#include "Core/SceneRuntimeHandler.h"

using namespace Hazard;
using namespace WindowLayout;

namespace WindowElement {

	void MenuBar::OnMenuRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);
		ImGui::BeginMainMenuBar();

		DrawMenu();

		bool sceneRunning = Runtime::SceneRuntimeHandler::IsSceneRunning();
		bool scenePaused = Runtime::SceneRuntimeHandler::IsScenePaused();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 16 * 3);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);

		Color onColor = Style::GetStyleColor(ColorType::Primary);
		Color offColor = Style::GetStyleColor(ColorType::Text);

		if (Input::ButtonColorChange(ICON_FK_PLAY, offColor, onColor, Style::GetStyleColor(ColorType::Background), sceneRunning, { 25, 25 })) {
			Runtime::SceneRuntimeHandler::SetSceneRunning(!sceneRunning);

			if (!sceneRunning) {}
				//EditorView::SetWindowFocus<GameViewport>();
			else {
				//EditorView::SetWindowFocus<Viewport>();
			}
		}
		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_PAUSE, offColor, onColor, Style::GetStyleColor(ColorType::Background), scenePaused, { 25, 25 })) {
			Runtime::SceneRuntimeHandler::SetScenePaused(!scenePaused);
		}
		Layout::SameLine(0, 5);

		if (Input::Button(ICON_FK_FORWARD, { 25, 25 })) 
		{
			if (sceneRunning && scenePaused)
				Application::GetModule<ScriptEngineManager>().Update();
		}

		ImGui::PopStyleVar();

		float width = ImGui::GetWindowWidth() - 550;
		ImGui::SetCursorPosY(0);

		ImGui::SetCursorPosX(width);
		ImGui::Text("%.2fms", 1000.0f / ImGui::GetIO().Framerate);

		ImGui::SetCursorPosX(width + 75);
		ImGui::Text("Mem: %.2fmb", Application::GetData().MemoryUsage);
		ImGui::SameLine(0, 15);

		width = ImGui::GetCursorPosX();
		ImGui::PushStyleColor(ImGuiCol_ChildBg, Style::ColorAsImVec4(Color::FromHex("#0D0D0B")));

		ImGui::SetCursorPosY(0);
		ImGui::SetCursorPosX(width + 125);
		ImGui::BeginChild("##projectName", { 200, 0 }, false);
		Project::ProjectManager* manager = &Application::GetModule<Project::ProjectManager>();

		std::string projectName = manager->GetProject().Name;
		ImGui::SetCursorPosX(ImGui::CalcTextSize(projectName.c_str()).x / 2.0f);
		ImGui::Text(projectName.c_str());

		ImGui::EndChild();
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
			EditorView::GetInstance().SetLayerActive<SettingsView>(true);
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::EndMainMenuBar();
	}

	bool MenuBar::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		bool returned = dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(MenuBar::KeyPressed));
		return false;
	}
	void MenuBar::DrawMenu()
	{
		Layout::Menu("File", []() {
			Layout::MenuItem("New", []() {});
			Layout::MenuItem("Open project", []() {
				std::string path = File::OpenFileDialog("Hazard project (*.hzrproj)\0*.hzrproj\0");

				if (!path.empty())
					Application::GetModule<Project::ProjectManager>().Load(path);
				});

			Layout::Separator();
			Layout::MenuItem("Save", []() {
				Application::GetModule<Project::ProjectManager>().Save();
				});
			Layout::MenuItem("Save as", []() {
				HZR_CORE_INFO("Save as TODO");
				});
			Layout::MenuItem("New world", []() {
				HZR_CORE_INFO("New world TODO"); 
				});
			Layout::Separator();
			Layout::MenuItem("Export", []() {
				Application::GetModule<EditorView>().GetRenderable<BuildWindow>()->SetActive(true);
				});
			Layout::Separator();
			Layout::MenuItem("Quit", []() {
				Application::Quit();
				});
			});
		Layout::Menu("Edit", []() {

			});
		Layout::Menu("View", []() {

			});
		Layout::Menu("Assets", []() {
			Layout::MenuItem("Reload script assemblies", []() {
				Application::GetModule<ScriptEngineManager>().ReloadAll();
				});
			Layout::MenuItem("Reload asset folder", []() {
				Application::GetModule<EditorView>().GetRenderable<FileView>()->UpdateFolderData();
				});
			});
		Layout::Menu("Window", []() {
			Layout::Menu("General", [&]() {
				Layout::MenuItem("Viewport", [&]() {
					Application::GetModule<EditorView>().GetRenderable<EditorMainTab>()->SetLayerActive<Viewport>(true);
					});
				Layout::MenuItem("Game viewport", [&]() {
					Application::GetModule<EditorView>().GetRenderable<EditorMainTab>()->SetLayerActive<GameViewport>(true);
					});
				Layout::MenuItem("Hierarchy", [&]() {
					Application::GetModule<EditorView>().GetRenderable<EditorMainTab>()->SetLayerActive<Hierarchy>(true);
					});
				Layout::MenuItem("Properties", [&]() {
					Application::GetModule<EditorView>().GetRenderable<EditorMainTab>()->SetLayerActive<Properties>(true);
					});
				Layout::MenuItem("Asset manager", [&]() {
					Application::GetModule<EditorView>().GetRenderable<EditorMainTab>()->SetLayerActive<FileView>(true);
					});
				Layout::MenuItem("Material inspector", [&]() {
					Application::GetModule<EditorView>().GetRenderable<EditorMainTab>()->SetLayerActive<MaterialInspector>(true);
					});
				});
			Layout::Menu("Rendering", [&]() {
				Layout::MenuItem("Environment", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<WorldEnvironmentData>(true);
					});
				});
			Layout::Menu("Debug", [&]() {
				Layout::MenuItem("Console", [&]() {
					Application::GetModule<EditorView>().GetRenderable<EditorMainTab>()->SetLayerActive<Console>(true);
					});
				Layout::MenuItem("Profiler", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<Profiler>(true);
					});
				Layout::MenuItem("Performance", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<Performance>(true);
					});
				Layout::MenuItem("Assets", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<EngineAssets>(true);
					});
				});
			});
		Layout::Menu("Help", []() {
			Layout::MenuItem("About", [&]() {

				});
			Layout::Menu("Theme", [&]() {
				Layout::MenuItem("Dark", [&]() {
					Style::InitTheme(Theme::Dark);
					});
				Layout::MenuItem("Classic", [&]() {
					Style::InitTheme(Theme::Classic);
					});
				});
			});
	}
	bool MenuBar::KeyPressed(KeyPressedEvent& e)
	{
		bool isCtrl = Hazard::Input::IsKeyDown(Key::LeftControl);
		if (!isCtrl)
			return false;

		if (e.GetKeyCode() == Key::S)
		{
			Application::GetModule<Project::ProjectManager>().SaveCurrentWorld();
			return true;
		}
		if (e.GetKeyCode() == Key::R)
		{
			Application::GetModule<Scripting::ScriptEngineManager>().ReloadAll();
			return true;
		}

		return false;
	}
}