#pragma once

#include <hzreditor.h>
#include "MenuBar.h"

#include "GUI/Library/Layout/Layout.h"
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
		ImGui::BeginMainMenuBar();

		Layout::Menu("File", []() {
			Layout::MenuItem("New", []() {});
			Layout::MenuItem("Open", []() {});
			Layout::Separator();
			Layout::MenuItem("Save", []() {
				Application::GetModule<Project::ProjectManager>().Save();
			});
			Layout::MenuItem("Save as", []() {});
			Layout::MenuItem("New scene", []() {});
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
				Application::GetModule<EditorView>().GetRenderable<FileView>()->UpdateFileTree();
				});
		});
		Layout::Menu("Window", []() {
			Layout::Menu("General", [&]() {
				Layout::MenuItem("Viewport", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<Viewport>(true);
					});
				Layout::MenuItem("Game viewport", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<GameViewport>(true);
					});
				Layout::MenuItem("Hierarchy", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<Hierarchy>(true);
					});
				Layout::MenuItem("Properties", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<Properties>(true);
					});
				Layout::MenuItem("Asset manager", [&]() {

					Application::GetModule<EditorView>().SetLayerActive<FileView>(true);
					});
				});
			Layout::Menu("Rendering", [&]() {
				Layout::MenuItem("Shader editor", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<ShaderEditorWindow>(true);
					});
				Layout::MenuItem("Environment", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<WorldEnvironmentData>(true);
					});
			});
			Layout::Menu("Debug", [&]() {
				Layout::MenuItem("Console", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<Console>(true);
					});
				Layout::MenuItem("Performance", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<Performance>(true);
					});
				Layout::MenuItem("Assets", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<EngineAssets>(true);
					});
				Layout::MenuItem("Script debugger", [&]() {
					Application::GetModule<EditorView>().SetLayerActive<ScriptDebug>(true);
					});
			});
		});
		Layout::Menu("Help", []() {
			Layout::MenuItem("About", [&]() {
				
			});
		});

		ImGui::EndMainMenuBar();
	}
	bool MenuBar::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(MenuBar::KeyPressed));
		return false;
	}
	bool MenuBar::KeyPressed(KeyPressedEvent& e)
	{
		bool isCtrl = Input::IsKeyDown(Key::LeftControl);
		if (!isCtrl) return false;

		switch (e.GetKeyCode()) {
		case Key::S:
			Application::GetModule<Project::ProjectManager>().SaveCurrentScene();
			HZR_CORE_WARN("Saving scene");
			return true;
		case Key::R:
			Application::GetModule<Scripting::ScriptEngineManager>().ReloadAll();
			HZR_CORE_WARN("Reloading assemblies");
			return true;
		}
		return false;
	}
}