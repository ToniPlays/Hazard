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
				std::string path = File::OpenFileDialog();

				if (path != "")
					Application::GetModule<Project::ProjectManager>()->Load(path);
				
			});
			Layout::Separator();
			Layout::MenuItem("Save", []() {
				Application::GetModule<Project::ProjectManager>()->Save();
			});
			Layout::MenuItem("Save as", []() {});
			Layout::MenuItem("New scene", []() {});
			Layout::Separator();
			Layout::MenuItem("Export", []() {
				Application::GetModule<EditorView>()->SetLayerActive<ExportView>(true);
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
				Application::GetModule<ScriptEngineManager>()->ReloadAll();
				});
			Layout::MenuItem("Reload asset folder", []() {
				Application::GetModule<EditorView>()->GetRenderable<FileView>()->UpdateFolderData();
				});
		});
		Layout::Menu("Window", []() {
			Layout::Menu("General", [&]() {
				Layout::MenuItem("Viewport", [&]() {
					Application::GetModule<EditorView>()->SetLayerActive<Viewport>(true);
					});
				Layout::MenuItem("Game viewport", [&]() {
					Application::GetModule<EditorView>()->SetLayerActive<GameViewport>(true);
					});
				Layout::MenuItem("Hierarchy", [&]() {
					Application::GetModule<EditorView>()->SetLayerActive<Hierarchy>(true);
					});
				Layout::MenuItem("Properties", [&]() {
					Application::GetModule<EditorView>()->SetLayerActive<Properties>(true);
					});
				Layout::MenuItem("Asset manager", [&]() {

					Application::GetModule<EditorView>()->SetLayerActive<FileView>(true);
					});
				});
			Layout::Menu("Rendering", [&]() {
				Layout::MenuItem("Shader editor", [&]() {
					Application::GetModule<EditorView>()->SetLayerActive<ShaderEditorWindow>(true);
					});
				Layout::MenuItem("Environment", [&]() {
					Application::GetModule<EditorView>()->SetLayerActive<WorldEnvironmentData>(true);
					});
			});
			Layout::Menu("Debug", [&]() {
				Layout::MenuItem("Console", [&]() {
					Application::GetModule<EditorView>()->SetLayerActive<Console>(true);
					});
				Layout::MenuItem("Profiler", [&]() {
					Application::GetModule<EditorView>()->SetLayerActive<Profiler>(true);
					});
				Layout::MenuItem("Performance", [&]() {
					Application::GetModule<EditorView>()->SetLayerActive<Performance>(true);
					});
				Layout::MenuItem("Assets", [&]() {
					Application::GetModule<EditorView>()->SetLayerActive<EngineAssets>(true);
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
		float width = ImGui::GetWindowWidth() - 250;

		ImGui::SetCursorPosX(width);
		std::stringstream ss;
		ss << "FPS: " << Math::Round(1.0f / Time::s_UnscaledDeltaTime, 1);
		ss << "/" << Math::Round(Time::s_UnscaledDeltaTime * 1000.0f, 2) << "ms";
		ImGui::Text(ss.str().c_str());
		ss.str("Mem: ");
		ss << Math::Round(Application::GetData().memoryUsage, 2) << " mb";

		ImGui::SetCursorPosX(width + 125);
		ImGui::Text(ss.str().c_str());

		/*ImGui::PopStyleVar();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
		Style::SetButtonColors("#181816", "#181816", "#181816");

		if (Input::Button(ICON_FK_WINDOW_MINIMIZE, { 25, 25 })) 
		{
			Application::GetModule<RenderContext>()->GetWindow().SetMinimized(true);
		}
		ImGui::SameLine(0, 0);
		if (Input::Button(ICON_FK_WINDOW_MAXIMIZE, { 25, 25 })) 
		{
			Window& window = Application::GetModule<RenderContext>()->GetWindow();
			window.SetMaximized(!window.IsMaximized());
		}
		ImGui::SameLine(0, 0);
		if (Input::Button(ICON_FK_TIMES, { 25, 25 })) 
		{
			Application::Quit();
		}

		ImGui::PopStyleColor(3);*/
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
		bool isSuper = Hazard::Input::IsKeyDown(Key::LeftAlt);
		if (e.GetKeyCode() == Key::Up)
		{
			Window& window = Application::GetModule<RenderContext>()->GetWindow();
			window.SetMaximized(!window.IsMaximized());
			return true;
		}

		bool isCtrl = Hazard::Input::IsKeyDown(Key::LeftControl);
		if (!isCtrl) 
			return false;

		if (e.GetKeyCode() == Key::S) 
		{
			Application::GetModule<Project::ProjectManager>()->SaveCurrentScene();
			HZR_WARN("Saving scene");
			return true;
		}
		if (e.GetKeyCode() == Key::R) 
		{
			Application::GetModule<Scripting::ScriptEngineManager>()->ReloadAll();
			HZR_WARN("Reloading assemblies");
			return true;
		}
		
		return false;
	}
}