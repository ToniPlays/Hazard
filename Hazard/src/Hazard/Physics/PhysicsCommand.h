#pragma once

#include "box2d/box2d.h"
#include "Hazard/Entity/World.h"

namespace Hazard::Physics 
{
	class PhysicsCommand {
	public:
		static void OnBeginRuntime(float gravity, ECS::World* world);
		static void OnEndRuntime();
		static void UpdateAll(ECS::World* world);
	private:
		static b2World* s_World2D;
	};
}