#pragma once

#include "Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "Core/RTCore/AccelerationStructure.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalBottomLevelAS : public BottomLevelAS
    {
    public:
        MetalBottomLevelAS(AccelerationStructureCreateInfo* info);
        ~MetalBottomLevelAS() = default;

        uint64_t GetCount() const override { return 0; };
        void PushTransforms(const BufferCopyRegion& copyRegion) override;

        void Invalidate() override;
        void Build(const BuildType& type) override;
        void Build(MTL::AccelerationStructureCommandEncoder* encoder, const BuildType& type);
        
        void Invalidate_RT();
    private:
        void CreateAccelerationStructure() {};
        void CreateTransformBuffer() {};

    private:
        std::string m_DebugName;
        AccelerationStructureLevel m_Level;
        std::vector<AccelerationStructureGeometry> m_Geometries;
        
        MTL::PrimitiveAccelerationStructureDescriptor* m_Descriptor;
        MTL::AccelerationStructure* m_Structure;
        MTL::Buffer* m_ScratchBuffer;
        
    };
}

#endif
