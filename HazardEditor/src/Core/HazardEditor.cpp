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

void HazardEditor::OnEvent(Hazard::Event& e)
{
	Hazard::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Hazard::KeyPressedEvent>(BIND(TestEvent));
}

bool HazardEditor::TestEvent(Hazard::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == GLFW_KEY_F11) {
		ImGuiLayer* layer = GetModuleHandler().GetModule<ImGuiLayer>();
		if (layer == nullptr) 
			return false;
		layer->SetActive(!layer->IsActive());
		return true;
	}

	return false;
}
