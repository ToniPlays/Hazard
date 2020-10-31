#pragma once
#include <hzrpch.h>
#include "ModuleHandler.h"
#include "Hazard/Modules/Renderer/RenderEngine.h"
#include "Hazard/Modules/Scene/SceneManager.h"
#include "Logging/Logger.h"

namespace Hazard {
	
	std::vector<Module*> ModuleHandler::modules;

	ModuleHandler::ModuleHandler()
	{
		//TEMP
#if !defined(HZR_RELEASE) && !defined(HZR_GAME_ONLY)
		PushModule(new Logger());
#endif // !HZR_GAME_ONLY || HZR_RELEASE

		PushModule(new RenderEngine());
		PushModule(new SceneManager());
	}
	ModuleHandler::~ModuleHandler()
	{
		OnClose();
	}

	void ModuleHandler::PushModule(Module* module)
	{
		if (module->OnEnabled()) { 
			modules.emplace_back(module);
			return;
		}
		HZR_CORE_WARN((std::string(module->GetName()) + " was not started correctly").c_str());

	}
	bool ModuleHandler::PopModule(Module* module)
	{
		auto it = std::find(modules.begin(), modules.end(), module);
		if (it != modules.end()) {
			modules.erase(it);
			if (!module->OnDisabled()) {
				HZR_CORE_WARN((std::string(module->GetName()) + " was not closed correctly").c_str());
				return false;
			}
			return true;
		}
		return false;
	}
	void ModuleHandler::Awake()
	{
		for (Module* module : modules) {
			if (module->IsActive())
				module->Awake();
		}
	}

	void ModuleHandler::Start()
	{
		for (Module* module : modules) {
			if (module->IsActive())
				module->Start();
		}
	}

	void ModuleHandler::Update()
	{
		PROFILE_FN();
		for (Module* module : modules) {
			if (module->IsActive())
				module->Update();
		}
		PROFILE_FN_END();
	}

	void ModuleHandler::LateUpdate()
	{
		PROFILE_FN();
		for (Module* module : modules) {
			if (module->IsActive())
				module->LateUpdate();
		}
		PROFILE_FN_END();
	}

	void ModuleHandler::OnRender()
	{
		PROFILE_FN();
		for (Module* module : modules) {
			if (module->IsActive())
				module->Render();
		}
		PROFILE_FN_END();
	}

	void ModuleHandler::OnEvent(Event& e)
	{

	}

	void ModuleHandler::OnClose()
	{
		for (Module* module : modules) {
			delete module;
		}
	}
}