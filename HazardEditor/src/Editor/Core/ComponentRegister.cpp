#pragma once
#include <hzreditor.h>
#include "ComponentRegister.h"

ComponentRegister::ComponentRegister() : Module("ComponentRegister") {

}
bool ComponentRegister::OnEnabled() {
	components.push_back(ComponentInfo("Transform", [](Entity* entity) { 
		entity->AddComponent<TransformComponent>(); 
	}));
	components.push_back(ComponentInfo("Camera", [](Entity* entity) {
		entity->AddComponent<CameraComponent>();
	}));
	components.push_back(ComponentInfo("Sprite renderer", [](Entity* entity) { 
		entity->AddComponent<SpriteRenderer>(); 
	}));
	components.push_back(ComponentInfo("BatchRenderer", [](Entity* entity) {
		entity->AddComponent<BatchRendererComponent>();
		}));
	return true;
}

void ComponentRegister::OnDestroy()
{
}
