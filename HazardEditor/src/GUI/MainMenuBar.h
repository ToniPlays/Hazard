#pragma once

#include "Core/MenuBar.h"
#include "Hazard.h"

namespace UI {
	class MainMenuBar : public MenuBar {
	public:
		MainMenuBar() {

			ClearMenuBar();

			AddMenuItem("File/New", nullptr);
			AddMenuItem("File/New/Project");
			AddMenuItem("File/New/Project/Crap/BunchOfShit", nullptr);
			AddMenuItem("File/Open/OpenNew", nullptr);
			AddMenuItem("File/Save", nullptr);
			AddMenuItem("File/Save as", nullptr);
			AddMenuItem("File/Project settings", nullptr);
			AddMenuItem("File/Quit", Hazard::Application::Quit);

			AddMenuItem("Edit/Undo", nullptr);
			AddMenuItem("Edit/Redo", nullptr);
			AddMenuItem("Edit/Cut", nullptr);
			AddMenuItem("Edit/Copy", nullptr);
			AddMenuItem("Edit/Paste", nullptr);

			AddMenuItem("Window/General", nullptr);
			AddMenuItem("Window/Layout", nullptr);
			
			AddMenuItem("Window/Layout", nullptr);

			AddMenuItem("Assets/Import", nullptr);
			AddMenuItem("Assets/Export", nullptr);

			AddMenuItem("Tools/World", nullptr);
			AddMenuItem("Tools/Audio", nullptr);
			AddMenuItem("Tools/Rendering", nullptr);

			AddMenuItem("Build/Settings", nullptr);
			AddMenuItem("Build/Output", nullptr);
			
			AddMenuItem("Help/About");
			AddMenuItem("Help/About/Something", nullptr);
		};
		~MainMenuBar() = default;

		virtual bool OnEvent(Event& e) { return false; }
	};
}