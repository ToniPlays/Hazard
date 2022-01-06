
#include "hzrpch.h"
#include "BufferBindings.h"
#include "Hazard/Events/Input.h"

#include "Hazard/Rendering/Pipeline/Buffers.h"

#include "Hazard/Scripting/ScriptResourceManager.h"
#include "Scripting/CSharp/Mono/Mono.h"

#include "mono/jit/jit.h"

namespace Hazard::Scripting::CSharp::Bindings {

    BindMap BufferBindings::GetBindings()
    {
        BindMap result;
        result.reserve(3);
        result.emplace_back("Hazard.Rendering.VertexBuffer::VertexBuffer_Create_Native", (void*) VertexBuffer_Create_Native);
        result.emplace_back("Hazard.Rendering.VertexBuffer::VertexBuffer_Destroy_Native", (void*) VertexBuffer_Destroy_Native);
        result.emplace_back("Hazard.Rendering.VertexBuffer::VertexBuffer_GetSize_Native", (void*) VertexBuffer_GetSize_Native);

        //result.emplace_back("Hazard.Rendering.IndexBuffer::IndexBuffer_Create_Native", )
        return result;
    }

    uint32_t BufferBindings::VertexBuffer_Create_Native(void* createInfo)
    {
        using namespace Hazard::Rendering;
        struct CSharpInfo {
            size_t size;
            BufferUsage usage;
            MonoArray* data;
        };

        CSharpInfo* cInfo = static_cast<CSharpInfo*>(createInfo);

        VertexBufferCreateInfo info = {};
        info.Size = cInfo->size;
        info.Usage = cInfo->usage;
        info.Data = cInfo->data ? Mono::GetArrayValuePointer(cInfo->data) : nullptr;
        
        return ScriptResourceManager::CreateResource<VertexBuffer>(&info);
    }

    void BufferBindings::VertexBuffer_Destroy_Native(uint32_t resourceID)
    {
        ScriptResourceManager::Destroy(resourceID);
    }

    uint32_t BufferBindings::VertexBuffer_GetSize_Native(uint32_t resourceID)
    {
        using namespace Hazard::Rendering;
        return ScriptResourceManager::GetResource<VertexBuffer>(resourceID)->GetSize();
    }
}
