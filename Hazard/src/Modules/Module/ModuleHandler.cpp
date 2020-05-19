#pragma once
#include <hzrpch.h>
#include "ModuleHandler.h"

namespace Hazard {

	ModuleHandler::ModuleHandler() {
	
	}
	ModuleHandler::~ModuleHandler() {
		for (Module* module : modules) {
			delete module;
		}
	}
	void ModuleHandler::PushModule(Module* module)
	{
		modules.emplace_back(module);
		module->OnEnable();
	}
	void ModuleHandler::PopModule(Module* module) {
		auto it = std::find(modules.begin(), modules.end(), module);
		if (it != modules.end()) {
			modules.erase(it);
			module->OnDisable();
		}
	}
	void ModuleHandler::Update()
	{
		for (Module* module : modules) {
			module->Update();
		}
	}
	void ModuleHandler::LateUpdate()
	{
		for (Module* module : modules) {
			module->LateUpdate();
		}
	}
	void ModuleHandler::OnRender()
	{
		for (Module* module : modules) {
			module->OnRender();
		}
	}
}
