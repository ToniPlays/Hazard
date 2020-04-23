#pragma once
#include "Core/EntryPoint.h"
#include "HazardEditor.h"
#include "EditorGui/Editor/EditorGUI.h"

HazardEditor::HazardEditor() : Hazard::Application("Editor")
{

}

HazardEditor::~HazardEditor()
{

}

void HazardEditor::Start()
{
	PushOverlay(new EditorGUI());
	Application::GetInfo().title = "Not at title";
}
