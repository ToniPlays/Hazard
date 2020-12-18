#pragma once

#include <hzrpch.h>
#include "SceneManager.h"
#include "Entity.h"

namespace Hazard {

	namespace ECS {

		SceneManager::SceneManager() : Module("SceneManager")
		{
			scene = new Scene();
		}
	}
}