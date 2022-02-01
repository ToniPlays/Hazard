
#include <hzrpch.h>
#include "ModuleHandler.h"
#include "Hazard/Logging/Logger.h"

namespace Hazard::Module {

	ModuleHandler::ModuleHandler()
	{
		m_Modules = std::vector<Module*>();
	}
	ModuleHandler::~ModuleHandler()
	{
		
	}

	void ModuleHandler::Update()
	{
		HZR_PROFILE_FUNCTION();
		for (Module* m : m_Modules)
			if (m->GetActive())
				m->Update();
	}
	void ModuleHandler::Render()
	{
		HZR_PROFILE_FUNCTION();
		for (Module* m : m_Modules)
			if (m->GetActive())
				m->Render();
	}
	void ModuleHandler::PostRender()
	{
		HZR_PROFILE_FUNCTION();
		for (Module* m : m_Modules)
			if (m->GetActive())
				m->PostRender();
	}
	void ModuleHandler::Close()
	{
		for (size_t i = m_Modules.size() - 1; i > 0; i--)
		{
			auto* mod = m_Modules[i];
			mod->Close();

			auto it = std::find(m_Modules.begin(), m_Modules.end(), mod);
			if (it != m_Modules.end())
			{
				m_Modules.erase(it);
				delete mod;
			}
		}
		m_Modules.clear();
	}
}
