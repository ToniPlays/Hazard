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
            delete m_Modules[i];
        }
    }

    void ModuleHandler::Update()
    {
        for (Module* m : m_Modules)
            if (m->GetActive())
                m->Update();
    }
    void ModuleHandler::PreRender()
    {
        for (Module* m : m_Modules)
            if (m->GetActive())
                m->PreRender();
    }
    void ModuleHandler::Render()
    {
        for (Module* m : m_Modules)
            if (m->GetActive())
                m->Render();
    }
    void ModuleHandler::PostRender()
    {
        for (Module* m : m_Modules)
            if (m->GetActive())
                m->PostRender();
    }
    void ModuleHandler::Close()
    {
        HZR_PROFILE_FUNCTION();
        for (Module* m : m_Modules)
            m->Close();
    }
}