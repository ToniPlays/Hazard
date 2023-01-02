#include "MetalBottomLevelAS.h"

#ifdef HZR_INCLUDE_METAL

#include "Renderer.h"
#include "MetalVertexBuffer.h"
#include "MetalIndexBuffer.h"

#include "MTLUtils.h"

namespace HazardRenderer::Metal
{
    MetalBottomLevelAS::MetalBottomLevelAS(AccelerationStructureCreateInfo* info)
    {
        m_Geometries.clear();
        m_Geometries.resize(info->GeometryCount);

        for (uint32_t i = 0; i < info->GeometryCount; i++)
            m_Geometries[i] = info->pGeometries[i];

        Invalidate();

        Ref<MetalBottomLevelAS> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            instance->Invalidate_RT();
            });
    }
    void MetalBottomLevelAS::PushTransforms(const BufferCopyRegion &copyRegion)
    {
        
    }
    void MetalBottomLevelAS::Invalidate()
    {
        
    }
    void MetalBottomLevelAS::Build(const BuildType& type)
    {
        auto device = MetalContext::GetMetalDevice();
        auto commandBuffer = device->GetGraphicsQueue()->commandBuffer();
        
        auto encoder = commandBuffer->accelerationStructureCommandEncoder();
        
        Build(encoder, type);
        
        encoder->endEncoding();
        commandBuffer->commit();
    }
    void MetalBottomLevelAS::Build(MTL::AccelerationStructureCommandEncoder* encoder, const BuildType& type)
    {
        auto device = MetalContext::GetMetalDevice()->GetMetalDevice();
        auto sizes = device->accelerationStructureSizes(m_Descriptor);
        
        m_ScratchBuffer = device->newBuffer(sizes.buildScratchBufferSize, MTL::ResourceOptionCPUCacheModeDefault);
        
        encoder->buildAccelerationStructure(m_Structure, m_Descriptor, m_ScratchBuffer, 0);
    }
    void MetalBottomLevelAS::Invalidate_RT()
    {
        auto device = MetalContext::GetMetalDevice();
        
        NS::Array* geometryDescriptors = NS::Array::alloc()->init();
        
        for(auto& mesh : m_Geometries)
        {
            auto* descriptor = MTL::AccelerationStructureTriangleGeometryDescriptor::alloc()->init();
            
            descriptor->setVertexBuffer(mesh.VertexBuffer.As<MetalVertexBuffer>()->GetMetalBuffer());
            
            descriptor->setIndexBuffer(mesh.IndexBuffer.As<MetalIndexBuffer>()->GetMetalBuffer());
            descriptor->setIndexType(MTL::IndexTypeUInt32);
            descriptor->setTriangleCount(mesh.IndexBuffer->GetCount());
        }
        
        auto* primitiveDescriptor = MTL::PrimitiveAccelerationStructureDescriptor::alloc()->init();
        primitiveDescriptor->setGeometryDescriptors(geometryDescriptors);
        
        m_Structure = device->GetMetalDevice()->newAccelerationStructure(primitiveDescriptor);
        
        SetDebugLabel(m_Structure, m_DebugName);
        
        m_Descriptor = primitiveDescriptor;
        Build(BuildType::Build);
    }
}
#endif
