
#include "MainMenuBar.h"
#include "AllPanels.h"
#include "Core/HazardEditor.h"
#include "Editor/EditorModeManager.h"
#include <Hazard/ImGUI/GUIManager.h>

namespace UI
{
	MainMenuBar::MainMenuBar()
	{
		Init();
	}
	void MainMenuBar::Init()
	{
		AddMenuItem("File/New", nullptr);
		AddMenuItem("File/New/Project");
		AddMenuItem("File/New/Project/Crap/BunchOfShit", nullptr);
		AddMenuItem("File/Open/OpenNew", nullptr);
		AddMenuItem("File/Save", [&]() {
			Ref<World> world = Application::Get().GetModule<WorldHandler>().GetCurrentWorld();
			//AssetManager::SaveAsset(world);
		});
		AddMenuItem("File/Save as", nullptr);
		AddMenuItem("File/Project settings", nullptr);
		AddMenuItem("File/Quit", []() { Hazard::Application::Get().Quit(); });

		AddMenuItem("Edit/Undo", nullptr);
		AddMenuItem("Edit/Redo", nullptr);
		AddMenuItem("Edit/Cut", nullptr);
		AddMenuItem("Edit/Copy", nullptr);
		AddMenuItem("Edit/Paste", nullptr);
		AddMenuItem("Edit/Reload assemblies", []() {
			((HazardEditorApplication&)Application::Get()).GetScriptManager().RecompileAndLoad();
		});

		AddMenuItem("Assets/Import", nullptr);
		AddMenuItem("Assets/Export", nullptr);

		AddMenuItem("Tools/Project settings", [&]() {
			//Application::Get().GetModule<Hazard::GUIManager>().SetPanelOpen<ProjectSettingsPanel>(true);
		});

		AddMenuItem("Tools/Task list", [&]() {
			//Application::Get().GetModule<GUIManager>().SetPanelOpen<ProjectTodoPanel>(true);
		});

		AddMenuItem("Tools/World", nullptr);
		AddMenuItem("Tools/Audio", nullptr);
		AddMenuItem("Tools/Rendering", nullptr);

        AddMenuItem("Build/Export package", [&]() {
            //Application::Get().GetModule<GUIManager>().SetPanelOpen<ExportPanel>(true);
        });
		AddMenuItem("Build/Show build folder", nullptr);

		AddMenuItem("Window/General/Hierarchy", [&]() {
			auto& manager = Application::Get().GetModule<GUIManager>();
			manager.GetExistingOrNew<Hierarchy>().BringToFront();
		});
		AddMenuItem("Window/General/Properties", [&]() {
			auto& manager = Application::Get().GetModule<GUIManager>();
			manager.GetExistingOrNew<Properties>().BringToFront();
		});
		AddMenuItem("Window/General/Asset panel", [&]() {
			auto& manager = Application::Get().GetModule<GUIManager>();
			manager.GetExistingOrNew<AssetPanel>().BringToFront();
		});

		AddMenuItem("Window/Rendering/Viewport", [&]() {
			auto& manager = Application::Get().GetModule<GUIManager>();
			manager.GetExistingOrNew<Viewport>().BringToFront();
		});
		AddMenuItem("Window/Rendering/Game viewport", [&]() {
			auto& manager = Application::Get().GetModule<GUIManager>();
			manager.GetExistingOrNew<GameViewport>().BringToFront();
		});

		AddMenuItem("Window/Debug/Console", [&]() {
			auto& manager = Application::Get().GetModule<GUIManager>();
			manager.GetExistingOrNew<Console>().BringToFront();
		});
		AddMenuItem("Window/Debug/Asset manager debugger", [&]() {
			auto& manager = Application::Get().GetModule<GUIManager>();
			manager.GetExistingOrNew<AssetManagerDebugPanel>().BringToFront();
		});
		AddMenuItem("Window/Debug/Render command list", [&]() {
			auto& manager = Application::Get().GetModule<GUIManager>();
			manager.GetExistingOrNew<RendererDebugPanel>().BringToFront();
		});
		AddMenuItem("Window/Debug/Performance debugger", [&]() {
			auto& manager = Application::Get().GetModule<GUIManager>();
			manager.GetExistingOrNew<PerformanceDebugPanel>().BringToFront();
		});

		AddMenuItem("Help/About");
	}
	bool MainMenuBar::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(MainMenuBar::OnKeyPressed));
	}
	bool MainMenuBar::OnKeyPressed(KeyPressedEvent& e)
	{
		if (!Input::IsKeyDown(Key::LeftControl)) return false;

		switch (e.GetKeyCode())
		{
			case Key::S:
			{
				using namespace Editor;
				using namespace Hazard;
				if (EditorModeManager::GetCurrentMode() == EditorMode::Edit)
				{
					auto& handler = Application::Get().GetModule<WorldHandler>();
					Ref<World> world = handler.GetCurrentWorld();
					SaveAssetSettings settings = {};

					if (world->GetSourceFilePath().empty())
					{
						std::string path = File::SaveFile({ "Hazard world (.hazard)", "*.hazard" });
						if (path.empty()) break;

						path = File::GetFileExtension(path) != ".hazard" ? path + ".hazard" : path;
						world->SetSourceFilePath(path);
						settings.TargetPath = File::GetPathNoExt(path).string() + ".hasset";
					}

					AssetManager::SaveAsset(world, settings);
				}
				return true;
			}
			case Key::P:
				using namespace Editor;
				if (EditorModeManager::GetCurrentMode() == EditorMode::Edit)
					EditorModeManager::BeginPlayMode();
				else
					EditorModeManager::EndPlayMode();
				return true;
		}
		return false;
	}
}
