#pragma once
#include "Core/EntryPoint.h"
#include "HazardEditor.h"
#include "EditorGUI.h"

HazardEditor::HazardEditor() : Hazard::Application("Editor")
{
	PushOverlay(new EditorGUI());
}

HazardEditor::~HazardEditor()
{

}
