
#include "MainMenuBar.h"
#include "GUI/GUIManager.h"
#include "AllPanels.h"
#include "Core/HazardEditor.h"
#include "Editor/EditorModeManager.h"

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
			Ref<World> world = Application::GetModule<WorldHandler>().GetCurrentWorld();
			//AssetManager::SaveAsset(world);
		});
		AddMenuItem("File/Save as", nullptr);
		AddMenuItem("File/Project settings", nullptr);
		AddMenuItem("File/Quit", Hazard::Application::Quit);

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
			Application::GetModule<GUIManager>().SetPanelOpen<ProjectSettingsPanel>(true);
		});

		AddMenuItem("Tools/Task list", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<ProjectTodoPanel>(true);
		});

		AddMenuItem("Tools/World", nullptr);
		AddMenuItem("Tools/Audio", nullptr);
		AddMenuItem("Tools/Rendering", nullptr);

        AddMenuItem("Build/Export package", [&]() {
            Application::GetModule<GUIManager>().SetPanelOpen<ExportPanel>(true);
        });
		AddMenuItem("Build/Show build folder", nullptr);

		AddMenuItem("Window/General/Hierarchy", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<Hierarchy>(true);
		});
		AddMenuItem("Window/General/Properties", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<Properties>(true);
		});
		AddMenuItem("Window/General/Asset panel", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<AssetPanel>(true);
		});

		AddMenuItem("Window/Rendering/Viewport", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<Viewport>(true);
		});
		AddMenuItem("Window/Rendering/Game viewport", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<GameViewport>(true);
		});

		AddMenuItem("Window/Debug/Console", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<Console>(true);
		});
		AddMenuItem("Window/Debug/Asset manager debugger", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<AssetManagerDebugPanel>(true);
		});
		AddMenuItem("Window/Debug/Render command list", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<RenderCommandListPanel>(true);
		});
		AddMenuItem("Window/Debug/World debugger", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<RendererDebugPanel>(true);
		});
		AddMenuItem("Window/Debug/Performance debugger", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<PerformanceDebugPanel>(true);
		});

		AddMenuItem("Help/About");
		AddMenuItem("Help/About/Something", nullptr);
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
					auto& handler = Application::GetModule<WorldHandler>();
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
