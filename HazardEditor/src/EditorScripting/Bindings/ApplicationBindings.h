#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "Core/HazardEditor.h"
#include <Editor/EditorModeManager.h>

namespace Editor::Bindings
{
	using namespace Hazard;

	static void Application_Quit_Native()
	{
		EditorMode mode = Editor::EditorModeManager::GetCurrentMode();
		if (mode == EditorMode::Play)
			Application::Get().SubmitMainThread([]() {
				Editor::EditorModeManager::EndPlayMode();
		});
	}
}
