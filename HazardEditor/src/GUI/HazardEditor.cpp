#pragma once
#include "Core/EntryPoint.h"
#include "HazardEditor.h"
#include "ImGui/Editor/EditorGUI.h"

HazardEditor::HazardEditor() : Hazard::Application("Editor")
{
	PushOverlay(new Hazard::EditorGUI());
}

HazardEditor::~HazardEditor()
{

}
