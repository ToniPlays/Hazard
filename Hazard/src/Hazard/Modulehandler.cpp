#pragma once

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
        for (uint32_t i = m_Modules.size() - 1; i > 0; i--) 
        {
            std::cout << m_Modules[i]->GetName();
            delete m_Modules[i];
        }
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
    void ModuleHandler::Close()
    {
        HZR_PROFILE_FUNCTION();
        for (Module* m : m_Modules)
            m->Close();
    }
}