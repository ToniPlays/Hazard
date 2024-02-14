#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include <Editor/EditorModeManager.h>
#include <Core/HazardEditor.h>
#include <Editor/GUI/Viewport/GameViewport.h>

namespace Editor::Bindings
{
	using namespace Hazard;

	static float Display_Width_Native()
	{
		return 0.0;
	}
	static float Display_Height_Native()
	{
		return 0.0f;
	}
}