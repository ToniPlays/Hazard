#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include <Editor/EditorModeManager.h>
#include <Core/HazardEditor.h>
#include "GUI/GUIManager.h"
#include <GUI/Viewport/GameViewport.h>

namespace Editor::Bindings
{
	using namespace Hazard;

	static float Display_Width_Native()
	{
		using namespace UI;
		Application& app = Application::Get();
		GameViewport* view = app.GetModule<GUIManager>().GetPanelManager().GetRenderable<GameViewport>();
		return (float)view->GetWidth();
	}
	static float Display_Height_Native()
	{
		using namespace UI;
		Application& app = Application::Get();
		GameViewport* view = app.GetModule<GUIManager>().GetPanelManager().GetRenderable<GameViewport>();
		return (float)view->GetHeight();
	}
}