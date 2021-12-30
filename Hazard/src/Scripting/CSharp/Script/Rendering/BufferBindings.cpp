#pragma once

#include "hzrpch.h"
#include "BufferBindings.h"
#include "Hazard/Events/Input.h"

namespace Hazard::Scripting::CSharp::Bindings {

    BindMap BufferBindings::GetBindings()
    {
        BindMap result;
        result.reserve(1);
        result.emplace_back("Hazard.Rendering.VertexBuffer::VertexBuffer_GetSize_Native", VertexBuffer_GetSize_Native);
        return result;
    }

    uint32_t BufferBindings::VertexBuffer_GetSize_Native(uint32_t resourceID)
    {
        return 0;
    }

}