
#include "MainMenuBar.h"
#include "Core/GUIManager.h"
#include "AllPanels.h"

namespace UI 
{
	MainMenuBar::MainMenuBar() 
	{
		ClearMenuBar();

		AddMenuItem("File/New", nullptr);
		AddMenuItem("File/New/Project");
		AddMenuItem("File/New/Project/Crap/BunchOfShit", nullptr);
		AddMenuItem("File/Open/OpenNew", nullptr);
		AddMenuItem("File/Save", [&]() {
			Ref<World> world = Application::GetModule<WorldHandler>().GetCurrentWorld();
			WorldSerializer serializer(world);
			serializer.SerializeEditor(world->GetWorldFile());

			});
		AddMenuItem("File/Save as", nullptr);
		AddMenuItem("File/Project settings", nullptr);
		AddMenuItem("File/Quit", Hazard::Application::Quit);

		AddMenuItem("Edit/Undo", nullptr);
		AddMenuItem("Edit/Redo", nullptr);
		AddMenuItem("Edit/Cut", nullptr);
		AddMenuItem("Edit/Copy", nullptr);
		AddMenuItem("Edit/Paste", nullptr);

		AddMenuItem("Assets/Import", nullptr);
		AddMenuItem("Assets/Export", nullptr);

		AddMenuItem("Tools/World", nullptr);
		AddMenuItem("Tools/Audio", nullptr);
		AddMenuItem("Tools/Rendering", nullptr);

		AddMenuItem("Build/Settings", nullptr);
		AddMenuItem("Build/Output", nullptr);

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
			//Application::GetModule<GUIManager>().SetPanelOpen<GameViewport>(true);
			});



		AddMenuItem("Window/Debug/Console", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<Console>(true);
			});
		AddMenuItem("Window/Debug/Render command list", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<RenderCommandListPanel>(true);
			});
		AddMenuItem("Window/Debug/World debugger", [&]() {
			Application::GetModule<GUIManager>().SetPanelOpen<RendererDebugPanel>(true);
			});

		AddMenuItem("Help/About");
		AddMenuItem("Help/About/Something", nullptr);
	};
}