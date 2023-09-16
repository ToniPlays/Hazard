#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "Hazard/ECS/WorldHandler.h"

namespace Hazard
{
	using namespace HazardScript;

	static bool WorldManager_LoadWorld_Native(uint64_t handle)
	{
		Ref<World> loadedWorld = AssetManager::GetAsset<World>(handle);
		if (!loadedWorld) 
			return false;

		auto& handler = Application::GetModule<WorldHandler>();
		handler.OnEnd();
		handler.SetWorld(loadedWorld);
		handler.OnBegin();
		return true;
	}
	/*
	static bool WorldManager_LoadWorldFromString_Native(MonoObject* tag)
	{
		return false;
	}
	*/
}