#pragma once

#include <hzreditor.h>
#include "ComponentView.h"
#include "VectorControl.h"
#include "Editor/GUI/GUILibrary/Inputs.h"
#include "Editor/Core/ComponentRegister.h"
#include "Editor/GUI/GUILibrary/Components.h"


void ComponentView::DrawComponents(Entity* entity) {

	for (Component* component : entity->GetComponents()) {
		Components::DrawComponent(entity, component);
	}
}
void ComponentView::ViewAll(Entity* entity) {

	std::vector<ComponentInfo> components = ModuleHandler::GetModule<ComponentRegister>()->GetAll();

	for (ComponentInfo info : components) {
		if (ImGui::MenuItem(info.path.c_str())) {
			info.fn(entity);
		}
	}
}