#pragma once
#include <hzreditor.h>
#include "ComponentRegister.h"
#include "Hazard.h"

ComponentRegister::ComponentRegister() : Module("ComponentRegister") {

}
bool ComponentRegister::OnEnabled() {
	components.push_back(ComponentInfo("Transform", [](Entity* entity) { 
		entity->AddComponent<Transform>(); 
	}));
	components.push_back(ComponentInfo("Sprite renderer", [](Entity* entity) { 
		entity->AddComponent<SpriteRenderer>(); 
	}));
	return true;
}