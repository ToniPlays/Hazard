#pragma once

#include "box2d/box2d.h"
#include "Hazard/Entity/World.h"
#include "Physics2D/Physics2D.h"

namespace Hazard::Physics 
{
	class PhysicsCommand {
	public:
		static void Init();
		static void OnBeginRuntime(float gravity, ECS::World* world);
		static void OnEndRuntime();
		static void UpdateAll(ECS::World* world);
	private:
		static Physics2D* s_Physics2D;
	};
}