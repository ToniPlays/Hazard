
#include "MetalTopLevelAS.h"

#ifdef HZR_INCLUDE_METAL

#include "MetalContext.h"
#include "MTLUtils.h"

#include "Renderer.h"

namespace HazardRenderer::Metal
{
    MetalTopLevelAS::MetalTopLevelAS(AccelerationStructureCreateInfo* info)
    {
        m_DebugName = info->DebugName;
        m_Level = info->Level;

        m_Instances.clear();
        m_Instances.resize(info->InstanceCount);

        for (uint32_t i = 0; i < info->InstanceCount; i++)
            m_Instances[i] = info->pInstances[i];

        Ref<MetalTopLevelAS> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            instance->Invalidate_RT();
            });
    }
    void MetalTopLevelAS::Invalidate()
    {
        if (m_Structure)
        {
            MTL::AccelerationStructure* structure = m_Structure;
            Ref<MetalTopLevelAS> instance = this;
            Renderer::SubmitResourceFree([structure]() mutable {
                structure->release();
            });
            
            Renderer::SubmitResourceCreate([instance]() mutable {
                instance->Invalidate_RT();
            });
            
            m_Structure = nullptr;
        }
    }
    void MetalTopLevelAS::Invalidate_RT() {
        
        auto device = MetalContext::GetMetalDevice();
        MTL::InstanceAccelerationStructureDescriptor* descriptor = MTL::InstanceAccelerationStructureDescriptor::alloc()->init();
        
        descriptor->setInstanceCount(m_Instances.size());
        descriptor->setUsage(MTL::AccelerationStructureUsagePreferFastBuild);
        
        m_Structure = device->GetMetalDevice()->newAccelerationStructure(descriptor);
        SetDebugLabel(m_Structure, m_DebugName);
    }
}
#endif
