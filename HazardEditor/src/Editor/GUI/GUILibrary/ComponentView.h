#pragma once
#include <hzreditor.h>

class ComponentView {

public:
	static void DrawComponents(Entity* entity);
	template<typename T, typename Draw, typename Context>
	static void DrawComponent(Entity* entity, Draw fn, Context ctx);
	static void ViewAll(Entity* entity);
};
