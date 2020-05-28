#pragma once
#include <hzrpch.h>
#include "ModuleHandler.h"
#include "Rendering/Renderer.h"
#include "Logging/Logger.h"

namespace Hazard {

	std::vector<Module*> ModuleHandler::modules = std::vector<Module*>();

	ModuleHandler::ModuleHandler()
	{
		//TEMP
		PushModule(new Logger);
		PushModule(new Renderer());
	}

	ModuleHandler::~ModuleHandler()
	{
		OnClose();
	}

	void ModuleHandler::PushModule(Module* module)
	{
		modules.emplace_back(module);
		module->OnEnabled();

	}
	bool ModuleHandler::PopModule(Module* module)
	{
		auto it = std::find(modules.begin(), modules.end(), module);
		if (it != modules.end()) {
			modules.erase(it);
			module->OnDisabled();
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
		PROFILE_FN();
	}

	void ModuleHandler::LateUpdate()
	{
		PROFILE_FN();
		for (Module* module : modules) {
			if (module->IsActive())
				module->LateUpdate();
		}
		PROFILE_FN();
	}

	void ModuleHandler::OnRender()
	{
		PROFILE_FN();
		for (Module* module : modules) {
			if (module->IsActive())
				module->Render();
		}
		PROFILE_FN();
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