
#include <hzrpch.h>
#include "PhysicsCommand.h"
#include "Hazard/ECS/Entity.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_world_callbacks.h"

namespace Hazard::Physics 
{
	std::string BodyTypeToString(BodyType type)
	{
		switch (type)
		{
		case BodyType::Static:		return "Static";
		case BodyType::Kinematic:	return "Kinematic";
		case BodyType::Dynamic:		return "Dynamic";
		}
		return "Unknows";
	}
	BodyType StringToBodyType(const std::string& value)
	{
		if (value == "Kinematic")	return BodyType::Kinematic;
		if (value == "Dynamic")		return BodyType::Dynamic;
		return BodyType::Static;
	}
	Physics2D* PhysicsCommand::s_Physics2D = nullptr;

	void PhysicsCommand::Init()
	{
		HZR_PROFILE_FUNCTION();
		s_Physics2D = new Physics2D();
	}
	void PhysicsCommand::BeginSimulation(PhysicsBeginInfo* info)
	{
		HZR_PROFILE_FUNCTION();
		s_Physics2D->Begin(info->Gravity);
	}
	void PhysicsCommand::StopSimulation()
	{
		HZR_PROFILE_FUNCTION();
		s_Physics2D->End();
	}
	void PhysicsCommand::Step(double delta)
	{
		HZR_PROFILE_FUNCTION();
		s_Physics2D->Step((float)delta);
	}
}
