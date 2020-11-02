#pragma once

#pragma once
#include <hzreditor.h>
#include "Inspector.h"
#include "Editor/GUI/GUILibrary/ComponentView.h"
#include "Editor/GUI/GUILibrary/Inputs.h"


using namespace Hazard;

Inspector::Inspector() : Layer("Properties")
{

}

Inspector::~Inspector()
{
	delete context;
}

void Inspector::Render()
{
	if (!Panel::Begin(name, isLayerOpen)) return;

	if (context != nullptr) {
		
		Inputs::InputField(context->name);
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add")) {
			ImGui::OpenPopup("AddComponent");
		}
		if (ImGui::BeginPopup("AddComponent")) {
			ComponentView::ViewAll(context);
			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Checkbox("Lock", &locked);
		ComponentView::DrawComponents(context);

		if (ImGui::BeginPopupContextWindow(0, 1, false)) {
			ComponentView::ViewAll(context);
			ImGui::EndPopup();
		}
	}
	
	Panel::End();
}
void Inspector::OpenContext(Entity* entity) {
	if (locked) return;
	context = entity;
}

