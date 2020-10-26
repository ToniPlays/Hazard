#pragma once

#include "Hazard.h"

class Components {
public:
	static void DrawComponent(Hazard::Entity* entity, Component* component);
	template<typename T, typename Draw, typename Context>
	static void DrawComponent(Entity* entity, Draw fn, Context ctx);
};