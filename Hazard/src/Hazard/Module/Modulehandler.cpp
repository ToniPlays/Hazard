#pragma once

#include <hzrpch.h>
#include "ModuleHandler.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Logging/Logger.h"

namespace Hazard::Module {

    ModuleHandler::ModuleHandler()
    {
        modules = std::vector<Module*>();
    }
    ModuleHandler::~ModuleHandler()
    {
        for (Module* m : modules) 
            delete m;
    }

    void ModuleHandler::PreInit()
    {
        /*for (Module* m : modules) 
            m->PreInit();*/
    }
    void ModuleHandler::Init()
    {
        /*for (Module* m : modules) 
            m->Init();*/
    }
    void ModuleHandler::Update()
    {
        for (Module* m : modules)
            if (m->GetActive())
                m->Update();
    }
    void ModuleHandler::Render()
    {
        for (Module* m : modules) 
            if (m->GetActive())
                m->Render();
    }
    void ModuleHandler::Close()
    {
        for (Module* m : modules) 
            m->Close();
    }
}