#pragma once
#include <hzrpch.h>
#include "ModuleHandler.h"
#include "Logging/Logger.h"
#include "ECS/SceneManager.h"

#include "Rendering/Core/RenderEngine.h"

namespace Hazard {
	
	std::vector<Module*> ModuleHandler::modules;

	ModuleHandler::ModuleHandler()
	{
		//TEMP
#if !defined(HZR_RELEASE)
		PushModule(new Logger());
#endif // !HZR_GAME_ONLY || HZR_RELEASE
		PushModule(new ECS::SceneManager());
		PushModule(new RenderEngine());

	}
	ModuleHandler::~ModuleHandler()
	{

	}

	void ModuleHandler::PushModule(Module* mod)
	{
		if (mod->OnEnabled()) {
			modules.emplace_back(mod);
			HZR_CORE_INFO(std::string(mod->GetName()) + " module enabled");
			return;
		}
		HZR_CORE_WARN(std::string(mod->GetName()) + " was not started correctly");

	}
	bool ModuleHandler::PopModule(Module* mod)
	{
		auto it = std::find(modules.begin(), modules.end(), mod);
		if (it != modules.end()) {
			modules.erase(it);
			if (mod->OnDisabled()) {
				return true;
			}
			HZR_CORE_WARN(std::string(mod->GetName()) + " was not closed correctly");
		}
		return false;
	}
	void ModuleHandler::Awake()
	{
		for (Module* module : modules) {
			if (module->IsActive()) {
				module->Awake();
			}
		}
	}

	void ModuleHandler::Start()
	{
		for (Module* module : modules) {
			if (module->IsActive()) {
				module->Start();
			}
		}
	}

	void ModuleHandler::Update()
	{
		PROFILE_FN();
		for (Module* module : modules) {
			if (module->IsActive()) {
				module->Update();
			}
		}
		PROFILE_FN_END();
	}

	void ModuleHandler::LateUpdate()
	{
		PROFILE_FN();
		for (Module* module : modules) {
			if (module->IsActive()) {
				module->LateUpdate();
			}
		}
		PROFILE_FN_END();
	}

	void ModuleHandler::OnRender()
	{
		PROFILE_FN();
		for (Module* module : modules) {
			if (module->IsActive()) {
				module->Render();
			}
		}
		PROFILE_FN_END();
	}

	void ModuleHandler::OnEvent(Event& e)
	{

	}

	void ModuleHandler::OnClose()
	{
		for (Module* module : modules) {
			module->OnDestroy();
		}
		modules.clear();
	}
}