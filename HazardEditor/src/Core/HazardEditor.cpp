#pragma once
#include "HazardEditor.h"
#include "ImGuiLayer.h"
#include "Hazard.h"

HazardEditor::HazardEditor() : Hazard::Application("Engine Editor") {
	
}

HazardEditor::~HazardEditor() {
	
}
void HazardEditor::Start() {

#ifndef HZR_GAME_ONLY
	GetModuleHandler().PushModule(new ImGuiLayer());
#endif

}
