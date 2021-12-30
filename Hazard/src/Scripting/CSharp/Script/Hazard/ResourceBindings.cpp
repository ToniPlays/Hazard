#pragma once

#include "hzrpch.h"
#include "ResourceBindings.h"
#include "Hazard/Events/Input.h"

namespace Hazard::Scripting::CSharp::Bindings {

    BindMap ResourceBindings::GetBindings()
    {
        BindMap result;
        result.reserve(1);
        result.emplace_back("Hazard.Resource::Resource_Create_Native", Resource_Create_Native);
        return result;
    }

    uint32_t ResourceBindings::Resource_Create_Native(void* type)
    {
        return 0;
    }
}