#pragma once
#include "Core/EntryPoint.h"
#include "HazardEditor.h"
#include "Editor/EditorGUI.h"

HazardEditor::HazardEditor() : Hazard::Application("Editor")
{

}

HazardEditor::~HazardEditor()
{

}

void HazardEditor::Start()
{
	Application::GetInfo().title = "Not at title";
#ifndef HZR_GAME_ONLY
	PushOverlay(new EditorGUI());
#endif
	//Debug::Log(Hazard::JSON::ReadFile(""));
}
