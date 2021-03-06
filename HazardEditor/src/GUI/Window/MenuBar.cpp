#pragma once

#include <hzreditor.h>
#include "MenuBar.h"

#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/Input.h"
#include "Project/ProjectManager.h"

#include "GUI/EditorView.h"
#include "GUI/Window/AllWindows.h"

using namespace Hazard;
using namespace WindowLayout;

namespace WindowElement {

	MenuBar::MenuBar() {}
	MenuBar::~MenuBar() {}

	void MenuBar::OnMenuRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);
		ImGui::BeginMainMenuBar();

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
			Layout::MenuItem("Save as", []() {});
			Layout::MenuItem("New scene", []() {});
			Layout::Separator();
			Layout::MenuItem("Export", []() {
				Application::GetModule<EditorView>().SetLayerActive<ExportView>(true);
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
		float width = ImGui::GetWindowWidth() - 475;

		ImGui::SetCursorPosX(width);
		ImGui::Text("FPS %.1f/%.2fms", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

		ImGui::SetCursorPosX(width + 130);
		ImGui::Text("Mem: %.2fmb", Application::GetData().memoryUsage);
		ImGui::SameLine(0, 15);
		width = ImGui::GetCursorPosX();
		ImGui::PushStyleColor(ImGuiCol_ChildBg, Style::ColorAsImVec4(Color::FromHex("#0D0D0B")));

		ImGui::BeginChild("##projectName", { 200, 0 }, false);
		Project::ProjectManager* manager = &Application::GetModule<Project::ProjectManager>();

		std::string projectName = manager->GetProject().m_Name;
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
	bool MenuBar::KeyPressed(KeyPressedEvent& e)
	{
		bool isCtrl = Hazard::Input::IsKeyDown(Key::LeftControl);
		if (!isCtrl)
			return false;

		if (e.GetKeyCode() == Key::S)
		{
			Application::GetModule<Project::ProjectManager>().SaveCurrentScene();
			HZR_WARN("Saving scene");
			return true;
		}
		if (e.GetKeyCode() == Key::R)
		{
			Application::GetModule<Scripting::ScriptEngineManager>().ReloadAll();
			HZR_WARN("Reloading assemblies");
			return true;
		}

		return false;
	}
}