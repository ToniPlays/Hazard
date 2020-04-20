#pragma once
#include "Core/EntryPoint.h"
#include "HazardEditor.h"

HazardEditor::HazardEditor() : Hazard::Application("Editor")
{
	PushOverlay(new Hazard::EditorGUI());
}

HazardEditor::~HazardEditor()
{

}
