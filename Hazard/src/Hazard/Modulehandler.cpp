
#include <hzrpch.h>
#include "ModuleHandler.h"
#include "Hazard/Logging/Logger.h"

namespace Hazard {

	ModuleHandler::ModuleHandler()
	{
		m_Modules = std::vector<Module*>();
	}
	ModuleHandler::~ModuleHandler() {}

	void ModuleHandler::InitializeAll()
	{
		for (Module* m : m_Modules)
			m->Init();

		m_HasInitialized = true;
	}
	void ModuleHandler::PreUpdate()
	{
		for (Module* m : m_Modules)
			if (m->IsActive())
				m->PreUpdate();
	}
	void ModuleHandler::Update()
	{
		for (Module* m : m_Modules)
			if (m->IsActive())
				m->Update();
	}
	void ModuleHandler::PostUpdate()
	{
		for (Module* m : m_Modules)
			if (m->IsActive())
				m->PostUpdate();
	}
	void ModuleHandler::PreRender()
	{
		for (Module* m : m_Modules)
			if (m->IsActive())
				m->PreRender();
	}
	void ModuleHandler::Render()
	{
		for (Module* m : m_Modules)
			if (m->IsActive())
				m->Render();
	}
	void ModuleHandler::PostRender()
	{
		for (Module* m : m_Modules)
			if (m->IsActive())
				m->PostRender();
	}
	void ModuleHandler::Close()
	{
		for (uint64_t i = m_Modules.size() - 1; i > 0; i--)
		{
			auto* mod = m_Modules[i];
			mod->Close();

			auto it = std::find(m_Modules.begin(), m_Modules.end(), mod);
			if (it != m_Modules.end())
			{
				m_Modules.erase(it);
				hdelete mod;
			}
		}
		m_Modules.clear();
	}
}
