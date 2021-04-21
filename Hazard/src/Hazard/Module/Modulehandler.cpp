#pragma once

#include <hzrpch.h>
#include "ModuleHandler.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Logging/Logger.h"

namespace Hazard::Module {

    ModuleHandler::ModuleHandler()
    {
        m_Modules = std::vector<Module*>();
    }
    ModuleHandler::~ModuleHandler()
    {
        for (Module* m : m_Modules)
            delete m;
    }

    void ModuleHandler::Update()
    {
        for (Module* m : m_Modules)
            if (m->GetActive())
                m->Update();
    }
    void ModuleHandler::Render()
    {
        for (Module* m : m_Modules)
            if (m->GetActive())
                m->Render();
    }
    void ModuleHandler::Close()
    {
        for (Module* m : m_Modules)
            m->Close();
    }
}